#pragma once


enum BallColor
{
    BC_NONE = 0,
    BC_RED,
    BC_ORANGE,
    BC_YELLOW,
    BC_GREEN,
    BC_BLUE,
    BC_INDIGO,
    BC_VIOLET,
    BC_WHITE,
    BC_BLACK,
    BC_LAST
};


enum BallState
{
    BS_GHOST, // маленький шарик-подсказка
    BS_NORMAL,
    BS_DEAD
};


class Ball : public LogicComponent
{
    OBJECT(Ball);

public:
    Ball(Context* context);
    static void RegisterObject(Context* context);
    virtual void Update(float timeStep);
    // Шар случайного цвета
    void Ball::Init(IntVector2 boardPosition, bool ghost);
    void Ball::Init(IntVector2 boardPosition, bool ghost, BallColor color);
    IntVector2 GetBoardPosition() const { return boardPosition_; }
    void SetBoardPosition(const IntVector2& position);
    void SetTargetPosition(const Vector3& targetPosition);
    BallColor GetColor() const { return color_; }
    BallState GetBallState() const { return ballState_; }
    void SetBallState(BallState state);
    void Kill();

private:
    //Cell* cell_;
    BallState ballState_;
    BallColor color_;
    IntVector2 boardPosition_;
    Material* ColorToMaterial(BallColor color);
};
