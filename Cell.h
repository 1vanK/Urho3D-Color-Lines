// Ячейка игрового поля


#pragma once


#define CELL_SIZE 2.0f


class Cell : public LogicComponent
{
    OBJECT(Cell);

public:
    Cell(Context* context);
    virtual void Update(float timeStep);
    static void RegisterObject(Context* context);
    void Init(const Vector3& startPosition, const Vector3& targetPosition, const IntVector2& boardPosition);
    IntVector2 GetBoardPosition() const { return boardPosition_; }

private:
    Vector3 targetPosition_;
    IntVector2 boardPosition_;
};
