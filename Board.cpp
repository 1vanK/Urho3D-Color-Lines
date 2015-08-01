#include "Urho3DAll.h"
#include "Board.h"
#include "Cell.h"
#include "Ball.h"
#include "Resources.h"
#include "Path.h"
#include <queue>


Board::Board(Context* context) :
    LogicComponent(context)
{
    SetUpdateEventMask(USE_UPDATE);
}


void Board::RegisterObject(Context* context)
{
    context->RegisterFactory<Board>();
}


Vector3 Board::IV2ToV3(IntVector2 iv2)
{
    // У массива board_ координата y увеличивается вниз, а в 3D пространстве - вверх, поэтому y меняет знак.
    // Так как координата ячейки обозначает ее центр, а не угол, то расстояние между
    // центрами первой и последней ячейки равно ширина/высота доски минус одна ячейка.
    Vector3 half((width_ - 1) * CELL_SIZE * 0.5f, -(height_ - 1) * CELL_SIZE * 0.5f, 0.0f);
    return Vector3(iv2.x_ * CELL_SIZE, -iv2.y_ * CELL_SIZE, 0.0f) - half;
}


bool Board::CreateBall(bool ghost, BallColor color)
{
    Node* ballNode = node_->CreateChild();
    Ball* ball = ballNode->CreateComponent<Ball>();
    IntVector2 boardPosition = RandomEmptyCell();
    if (boardPosition.x_ == -1)
        return false;
    int x = boardPosition.x_;
    int y = boardPosition.y_;
    Vector3 targetPos = IV2ToV3(boardPosition);
    Vector3 startPos = Vector3(Random(-50.0f, 50.0f), Random(-50.0f, 50.0f), -50.0f);
    ball->Init(boardPosition, ghost, color);
    board_[y][x] = ball;
    needCheckLines_ = true;
    return true;
}


bool Board::CreateBall(bool ghost)
{
    Node* ballNode = node_->CreateChild();
    Ball* ball = ballNode->CreateComponent<Ball>();
    IntVector2 boardPosition = RandomEmptyCell();
    if (boardPosition.x_ == -1)
        return false;
    int x = boardPosition.x_;
    int y = boardPosition.y_;
    Vector3 targetPos = IV2ToV3(boardPosition);
    Vector3 startPos = Vector3(Random(-50.0f, 50.0f), Random(-50.0f, 50.0f), -50.0f);
    ball->Init(boardPosition, ghost);
    board_[y][x] = ball;
    needCheckLines_ = true;
    return true;
}


bool Board::SpawnBall()
{
    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            if (board_[y][x] && board_[y][x]->GetBallState() == BS_GHOST)
            {
                board_[y][x]->SetBallState(BS_NORMAL);
                needCheckLines_ = true;
                return true;
            }
        }
    }

    return CreateBall(false);
}


void Board::SetDifficulty(Difficulty difficulty)
{
    width_ = 9;
    height_ = 9;
    numColors_ = 7;
    numAddBalls_ = 3;
    lineLength_ = 5;
    showNext_ = true;
    difficulty_ = difficulty;

    if (difficulty == D_EASY)
    {
        numColors_ = 5;
    }
    else if (difficulty == D_HARD)
    { 
        showNext_ = false;
    }
    else if (difficulty == D_EXPERT)
    {
        showNext_ = false;
        numColors_ = 8;
        lineLength_ = 8;
        width_ = 16;
        height_ = 16;
    }
    else if (difficulty == D_MASTER)
    {
        showNext_ = false;
        numColors_ = 9;
        lineLength_ = 9;
        width_ = 20;
        height_ = 20;
        numAddBalls_ = 4;
    }
}


void Board::InitSelection()
{
    selectedBall_ = nullptr;
    selectionNode_ = node_->CreateChild("Selection");
    selectionNode_->SetRotation(Quaternion(-90.f, 0.0f, 0.0f));
    selectionNode_->SetEnabled(false);
    StaticModel* staticModel = selectionNode_->CreateComponent<StaticModel>();
    staticModel->SetModel(SelectionModel);
    staticModel->SetMaterial(0, SelectionMaterial0);
    staticModel->SetMaterial(1, SelectionMaterial1);
    staticModel->SetCastShadows(true);
}


Cell* Board::GetCell(IntVector2 iv2)
{
    return cells_[iv2.y_][iv2.x_];
}


void Board::Init(Difficulty difficulty)
{
    SetDifficulty(difficulty);
    node_->SetPosition(Vector3(0.0f, 0.0f, Max(width_, height_) * 2.8f));

    needCheckLines_ = false;
    needSpawnBalls_ = false;
    gameOver_ = false;
    score_ = 0;
    
    board_.Resize(height_);
    cells_.Resize(height_);
    for (int i = 0; i < height_; i++)
    {
        board_[i].Resize(width_);
        cells_[i].Resize(width_);
        for (int j = 0; j < width_; j++)
        {
            Node* cellNode = node_->CreateChild();
            Cell* cell = cellNode->CreateComponent<Cell>();
            Vector3 targetPos = IV2ToV3(IntVector2(j, i));
            Vector3 startPos = Vector3(Random(-50.0f, 50.0f), Random(-50.0f, 50.0f), Random(-50.0f, 50.0f));
            cell->Init(startPos, targetPos, IntVector2(j, i));
            cells_[i][j] = cell;
        }
    }

    for (int i = 0; i < numAddBalls_; i++)
        SpawnBall();

    if (difficulty_ <= D_NORMAL)
    {
        for (int i = 0; i < numAddBalls_; i++)
            CreateBall(true);
    }

    InitSelection();
}

void Board::Restart()
{
    needCheckLines_ = false;
    needSpawnBalls_ = false;
    gameOver_ = false;
    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            if (board_[i][j])
            {
                board_[i][j]->GetNode()->Remove();
                board_[i][j] = nullptr;
            }
        }
    }
    selectedBall_ = nullptr;

    for (int i = 0; i < numAddBalls_; i++)
        SpawnBall();

    if (difficulty_ <= D_NORMAL)
    {
        for (int i = 0; i < numAddBalls_; i++)
            CreateBall(true);
    }

    score_ = 0;

}


IntVector2 Board::RandomEmptyCell()
{
    Vector<IntVector2> emptyCells_;
    emptyCells_.Reserve(width_ * height_);
    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            if (board_[y][x] == nullptr)
                emptyCells_.Push(IntVector2(x, y));
        }
    }
    if (emptyCells_.Size() == 0)
        return IntVector2(-1, -1);
    int rnd = Random(0, emptyCells_.Size());
    return emptyCells_[rnd];
}


void Board::HandleClickBall(Ball* ball)
{
    if (Path::GetTotalCount() != 0)
        return;
    /*IntVector2 pos = ball->GetBoardPosition();
    board_[pos.y_][pos.x_] = nullptr;
    ball->GetNode()->Remove();*/
    if (ball->GetBallState() == BS_DEAD)
        return;
    if (ball->GetBallState() == BS_GHOST)
    {
        Cell* cell = GetCell(ball->GetBoardPosition());
        HandleClickCell(cell);
        return;
    }
    if (ball == selectedBall_)
    {
        selectedBall_ = nullptr;
        selectionNode_->SetEnabled(false);
    }
    else
    {
        selectedBall_ = ball;
        selectionNode_->SetEnabled(true);
        selectionNode_->SetPosition(ball->GetNode()->GetPosition());
    }
}


void Board::HandleClickCell(const Cell* cell)
{
    if (Path::GetTotalCount() != 0)
        return;
    IntVector2 pos = cell->GetBoardPosition();
    if (board_[pos.y_][pos.x_] && board_[pos.y_][pos.x_]->GetBallState() == BS_NORMAL)
    {
        HandleClickBall(board_[pos.y_][pos.x_]);
    }
    else if (selectedBall_)
    {
        IntVector2 oldPos = selectedBall_->GetBoardPosition();
        IntVector2 newPos = pos;
        Vector<IntVector2> boardPath;
        if (FindPath(oldPos, newPos, boardPath))
        {
            bool needCreateGhost = false;
            BallColor color;
            if (board_[newPos.y_][newPos.x_]) // в точке назначения может быть призрак
            {
                color = board_[newPos.y_][newPos.x_]->GetColor();
                board_[newPos.y_][newPos.x_]->Kill(); // убиваем
                needCreateGhost = true;
            }
            board_[newPos.y_][newPos.x_] = selectedBall_;
            selectedBall_->SetBoardPosition(newPos);
            board_[oldPos.y_][oldPos.x_] = nullptr;
            Path* path = selectedBall_->GetNode()->CreateComponent<Path>();
            path->AddControlPoint(IV2ToV3(oldPos));
            for (int i = 0; i < boardPath.Size(); i++)
                path->AddControlPoint(IV2ToV3(boardPath[i]));
            path->SetSpeed(80.0f);
            selectedBall_ = nullptr;
            selectionNode_->SetEnabled(false);
            needCheckLines_ = true;
            needSpawnBalls_ = true;
            if (needCreateGhost)
                CreateBall(true, color); // создаем призрака в новом месте того же цвета
        }
    }
}


//Проверить можно ли через ячейку in двигатся шару
bool Board::Valid(const IntVector2 &in)
{
    if (in.x_ < 0 || in.x_ >= width_) // выход за границы
        return false;
    if (in.y_ < 0 || in.y_ >= height_)
        return false;
    if (board_[in.y_][in.x_] && board_[in.y_][in.x_]->GetBallState() == BS_NORMAL) // уже занята
        return false;
    return true;
}



bool Board::FindPath(const IntVector2& start, const IntVector2& end, Vector<IntVector2>& path)
{
    struct st
    {
        IntVector2 pred; // предыдущая клетка, ведущая к началу
        int mark; // обработана ли ячейка (то есть найден ли к ней путь от стартовой)
    };
    Vector<Vector<st> > v;
    v.Resize(height_);
    for (int i = 0; i < height_; i++)
    {
        v[i].Resize(width_);
        for (int j = 0; j < width_; j++)
        {
            v[i][j].mark = 0; // все ячейки необработаны
        }
    }
    std::queue<IntVector2> q; // нужно обработать соседей данных клеток
    v[start.y_][start.x_].mark = 1; // стартовую клетка обработана сразу (известен путь от стартовой до стартовой :)
    q.push(start); // нужно обработать всех соседей стартовой кнопки
    while (!q.empty()) // если очередь, то путь не найден
    {
        IntVector2 k = q.front();
        for (int i = 0; i<4; i++) // 4 соседа вокруг клетки
        {
            IntVector2 l = k; // очередной сосед
            if (i == 0)
                l.x_--;
            else if (i == 1)
                l.x_++;
            else if (i == 2)
                l.y_--;
            else
                l.y_++;
            if (Valid(l) && !v[l.y_][l.x_].mark)
            {
                v[l.y_][l.x_].mark = 1;
                v[l.y_][l.x_].pred = k;
                q.push(l);

                if (l == end) // нашли конечную точку
                {
                    do
                    {
                        path.Insert(0, l);
                        l = v[l.y_][l.x_].pred;

                    } while (l != start);
                    return true;
                }
            }
        }
        q.pop();
    }
    return false;
}


void Board::HandleClickSelection()
{
    if (Path::GetTotalCount() != 0)
        return;
    selectedBall_ = nullptr;
    selectionNode_->SetEnabled(false);
}


void Board::Update(float timeStep)
{
    if (gameOver_)
    {
        Restart();
        return;
    }

    selectionNode_->Rotate(Quaternion(0.0f, timeStep * 50, 0.0f));
    if (Path::GetTotalCount() > 0)
        return;
    if (needCheckLines_)
    {
        if (CheckLines())
        {
            needSpawnBalls_ = false;
        }
        needCheckLines_ = false;
        // может быть что поле полностью занято, новых шаров спанить не надо и ходить некуда
        // не работает чот
        gameOver_ = true;
        for (int i = 0; i < height_; i++)
        {
            for (int j = 0; j < width_; j++)
            {
                if (!board_[i][j] || board_[i][j]->GetBallState() == BS_GHOST)
                {
                    gameOver_ = false;
                    break;
                }

            }
        }
        if (gameOver_)
            return;

    }
    if (needSpawnBalls_)
    {
        for (int i = 0; i < numAddBalls_; i++)
        {
            if (!SpawnBall())
            {
                gameOver_ = true;
                return;
            }
        }
        if (difficulty_ <= D_NORMAL)
        {
            for (int i = 0; i < numAddBalls_; i++)
                CreateBall(true);
        }
        needSpawnBalls_ = false;
    }

    UIElement* uiRoot = GetSubsystem<UI>()->GetRoot();
    Text* t = static_cast<Text*>(uiRoot->GetChild("Score", true));
    t->SetText("Score: " + String(score_));
    t = static_cast<Text*>(uiRoot->GetChild("Colors", true));
    t->SetText("Colors: " + String(numColors_));
    t = static_cast<Text*>(uiRoot->GetChild("LineLength", true));
    t->SetText("Line length: " + String(lineLength_));

    // нужно выбрасывать шары если после уделаения линий поле пустое
}


bool Board::CheckLines()
{
    Vector<Vector<bool > > exploded;

    exploded.Resize(height_);
    for (int i = 0; i < height_; i++)
    {
        exploded[i].Resize(width_);
        for (int j = 0; j < width_; j++)
        {
            exploded[i][j] = false;
        }
    }

    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            if (!board_[i][j] || board_[i][j]->GetBallState() != BS_NORMAL)
                continue;
            BallColor color = board_[i][j]->GetColor();
            int length = 1;
            for (int k = i - 1; k >= 0; k--) // по веритикали
            {
                if (!board_[k][j] || board_[k][j]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[k][j]->GetColor() == color)
                    length++;
                else
                    break;
            }
            for (int k = i + 1; k < height_; k++)
            {
                if (!board_[k][j] || board_[k][j]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[k][j]->GetColor() == color)
                    length++;
                else
                    break;
            }
            if (length >= lineLength_)
            {
                exploded[i][j] = true;
                continue;
            }
            length = 1;
            for (int k = j - 1; k >= 0; k--) // по горизонтали
            {
                if (!board_[i][k] || board_[i][k]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[i][k]->GetColor() == color)
                    length++;
                else
                    break;
            }
            for (int k = j + 1; k < width_; k++)
            {
                if (!board_[i][k] || board_[i][k]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[i][k]->GetColor() == color)
                    length++;
                else
                    break;
            }
            if (length >= lineLength_)
            {
                exploded[i][j] = true;
                continue;
            }
            length = 1;
            for (int k = i - 1, m = j - 1; k >= 0 && m >= 0; k--, m--) // по диагонали 1
            {
                if (!board_[k][m] || board_[k][m]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[k][m]->GetColor() == color)
                    length++;
                else
                    break;
            }
            for (int k = i + 1, m = j + 1; k < height_ && m < width_; k++, m++)
            {
                if (!board_[k][m] || board_[k][m]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[k][m]->GetColor() == color)
                    length++;
                else
                    break;
            }
            if (length >= lineLength_)
            {
                exploded[i][j] = true;
                continue;
            }
            length = 1;
            for (int k = i - 1, m = j + 1; k >= 0 && m < width_; k--, m++) // по диагонали 2
            {
                if (!board_[k][m] || board_[k][m]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[k][m]->GetColor() == color)
                    length++;
                else
                    break;
            }
            for (int k = i + 1, m = j - 1; k < height_ && m >= 0; k++, m--)
            {
                if (!board_[k][m] || board_[k][m]->GetBallState() != BS_NORMAL)
                    break;
                if (board_[k][m]->GetColor() == color)
                    length++;
                else
                    break;
            }
            if (length >= lineLength_)
            {
                exploded[i][j] = true;
                continue;
            }

        }
    }

    int totalExploded = 0;

    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            if (exploded[i][j])
            {
                totalExploded++;
                board_[i][j]->Kill();
                board_[i][j] = nullptr;
            }
        }
    }

    score_ += totalExploded;
    if (totalExploded > lineLength_) // за каждый шарик выше минимума дополнительное очко
    {
        score_ += totalExploded - lineLength_;
    }

    if (totalExploded > 0)
        return true;

    return false;
}