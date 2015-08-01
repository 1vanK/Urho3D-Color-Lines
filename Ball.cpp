#include "Urho3DAll.h"
#include "Ball.h"
#include "Resources.h"
#include "Path.h"
#include "Board.h"


Ball::Ball(Context* context) :
    LogicComponent(context),
    color_(BC_NONE)
{
    SetUpdateEventMask(USE_UPDATE);
}


void Ball::RegisterObject(Context* context)
{
    context->RegisterFactory<Ball>();
}


void Ball::SetBoardPosition(const IntVector2& position)
{
    boardPosition_ = position;
}


void Ball::Init(IntVector2 boardPosition, bool ghost, BallColor color)
{
    boardPosition_ = boardPosition;

    if (ghost)
        ballState_ = BS_GHOST;
    else
        ballState_ = BS_NORMAL;

    Board* board = node_->GetParent()->GetComponent<Board>();
    node_->SetName("Ball");
    node_->SetPosition(board->IV2ToV3(boardPosition));
    node_->SetRotation(Quaternion(-90.f, 0.0f, 0.0f));
    node_->SetScale(0.0f);
    color_ = color;
    StaticModel* staticModel = node_->CreateComponent<StaticModel>();
    staticModel->SetModel(BallModel);
    staticModel->SetMaterial(ColorToMaterial(color_));
    staticModel->SetCastShadows(true);
}

void Ball::Init(IntVector2 boardPosition, bool ghost)
{
    boardPosition_ = boardPosition;

    if (ghost)
        ballState_ = BS_GHOST;
    else
        ballState_ = BS_NORMAL;

    Board* board = node_->GetParent()->GetComponent<Board>();
    node_->SetName("Ball");
    node_->SetPosition(board->IV2ToV3(boardPosition));
    node_->SetRotation(Quaternion(-90.f, 0.0f, 0.0f));
    node_->SetScale(0.0f);
    color_ = (BallColor)Random(1, board->GetNumColors() + 1);
    StaticModel* staticModel = node_->CreateComponent<StaticModel>();
    staticModel->SetModel(BallModel);
    staticModel->SetMaterial(ColorToMaterial(color_));
    staticModel->SetCastShadows(true);
}


void Ball::Update(float timeStep)
{
    Node* node = GetNode();
    if (ballState_ == BS_DEAD)
    {
        float scale = node->GetScale().x_;
        scale -= timeStep * 2;
        if (scale <= 0.001f)
            node->Remove();
        else
            node->SetScale(scale);
    }
    else if (ballState_ == BS_GHOST)
    {
        float scale = GetNode()->GetScale().x_;
        scale += timeStep * 2;
        if (scale > 0.25f)
            scale = 0.25f;
        node->SetScale(scale);
    }
    else
    {
        float scale = GetNode()->GetScale().x_;
        scale += timeStep * 2;
        if (scale > 1.0f)
            scale = 1.0f;
        node->SetScale(scale);
    }
}


Material* Ball::ColorToMaterial(BallColor color)
{
    if (color == BC_RED)
        return RedBallMaterial;
    if (color == BC_ORANGE)
        return OrangeBallMaterial;
    if (color == BC_YELLOW)
        return YellowBallMaterial;
    if (color == BC_GREEN)
        return GreenBallMaterial;
    if (color == BC_BLUE)
        return BlueBallMaterial;
    if (color == BC_INDIGO)
        return IndigoBallMaterial;
    if (color == BC_VIOLET)
        return VioletBallMaterial;
    if (color == BC_BLACK)
        return BlackBallMaterial;
    if (color == BC_WHITE)
        return WhiteBallMaterial;
    return nullptr;
}


void Ball::Kill()
{
    ballState_ = BS_DEAD;
    boardPosition_ = IntVector2(-1, -1);
}


void Ball::SetBallState(BallState state)
{
    ballState_ = state;
}
