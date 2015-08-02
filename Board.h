#pragma once


#include "Ball.h"
#include "Cell.h"


enum Difficulty
{
    D_EASY = 0,
    D_NORMAL,
    D_HARD,
    D_EXPERT,
    D_MASTER
};


#define BOARD_MAX_SIZE 9
#define BOARD_MIN_SIZE 9


class Board : public LogicComponent
{
    OBJECT(Board);

public:
    Board(Context* context);
    static void RegisterObject(Context* context);
    void Init(Difficulty difficulty);
    Vector3 IV2ToV3(IntVector2 iv2);
    void HandleClickBall(Ball* ball);
    void HandleClickCell(const Cell* cell);
    Cell* GetCell(IntVector2 iv2);
    void HandleClickSelection();
    virtual void Update(float timeStep);
    void Restart();
    int GetNumColors() const { return numColors_; }

private:
    Difficulty difficulty_;
    int width_;
    int height_;
    int numColors_;
    int numAddBalls_;
    int lineLength_;
    bool showNext_;
    int score_;
    Vector<Vector<SharedPtr<Ball> > > board_;
    Vector<Vector<SharedPtr<Cell> > > cells_;
    Node* selectionNode_;
    Ball* selectedBall_;
    bool needCheckLines_;
    bool needSpawnBalls_;
    bool gameOver_;

    void SetDifficulty(Difficulty difficulty);
    void InitSelection();
    IntVector2 RandomEmptyCell();
    bool SpawnBall();
    bool CreateBall(bool ghost);
    bool CreateBall(bool ghost, BallColor color);
    bool FindPath(const IntVector2& start, const IntVector2& end, Vector<IntVector2>& path);
    bool Valid(const IntVector2 &in);
    bool CheckLines();

    void SaveRecord();
    void LoadRecord();
    int record_;

    String GetRecordsFileName();

};
