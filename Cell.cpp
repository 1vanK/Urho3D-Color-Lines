#include "Urho3DAll.h"
#include "Cell.h"
#include "Resources.h"
#include "Path.h"


Cell::Cell(Context* context) :
    LogicComponent(context)
{
    SetUpdateEventMask(USE_UPDATE);
}


void Cell::RegisterObject(Context* context)
{
    context->RegisterFactory<Cell>();
}


void Cell::Init(const Vector3& startPosition, const Vector3& targetPosition, const IntVector2& boardPosition)
{
    node_->SetName("Cell");
    StaticModel* staticModel = node_->CreateComponent<StaticModel>();
    staticModel->SetModel(CellModel);
    staticModel->SetMaterial(CellMaterial);
    staticModel->SetCastShadows(true);
    node_->SetPosition(startPosition);
    node_->SetRotation(Quaternion(-90.f, 0.0f, 0.0f));
    node_->SetScale(0.0f);
    targetPosition_ = targetPosition;
    boardPosition_ = boardPosition;
    Path* path = node_->CreateComponent<Path>();
    path->AddControlPoint(startPosition);
    path->AddControlPoint(targetPosition);
    path->SetSpeed(80.0f);
}


void Cell::Update(float timeStep)
{
    // ������ ����� ����� ��� �� �������� ����� ����, � ����� � ������� ������-�� ���������� ������� ������ ���������� �� 1.0f.
    // ��������� ����� ���� ���� ����� ������ ��������� � ������� ���������, ��� �� �������� ��� ��������� ������������� ������.
    float scale = GetNode()->GetScale().x_;
    scale += timeStep * 2.0f;
    if (scale > 1.0)
        scale = 1.0f;
    GetNode()->SetScale(scale);
}
