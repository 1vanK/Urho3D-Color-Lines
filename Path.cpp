#include "Urho3DAll.h"
#include "Path.h"


Path::Path(Context* context) :
    LogicComponent(context),
    length_(0.0f),
    speed_(0.0f),
    traveled_(0.0f)
{
    SetUpdateEventMask(USE_UPDATE);
    totalCount_++;
}


Path::~Path()
{
    totalCount_--;
}


void Path::RegisterObject(Context* context)
{
    context->RegisterFactory<Path>();
}


void Path::SetSpeed(float speed)
{
    speed_ = speed;
}


void Path::AddControlPoint(const Vector3& point)
{
    controlPoints_.Push(point);
    int count = controlPoints_.Size();
    if (count > 1)
        length_ += (controlPoints_[count - 1] - controlPoints_[count - 2]).Length();
}


void Path::Update(float timeStep)
{
    traveled_ += timeStep * speed_;
    int size = controlPoints_.Size();
    if (traveled_ >= length_)
    {
        node_->SetPosition(controlPoints_[size - 1]);
        node_->RemoveComponent(this);
        return;
    }
    int segment = 1;
    float sumLength = 0.0f;
    for (; segment < size; segment++)
    {
        float segmentLength = (controlPoints_[segment] - controlPoints_[segment - 1]).Length();
        sumLength += segmentLength;
        if (sumLength >= traveled_)
            break;
    }
    Vector3 direction = (controlPoints_[segment - 1] - controlPoints_[segment]);
    direction.Normalize();
    Vector3 pos = direction * (sumLength - traveled_) + controlPoints_[segment];
    node_->SetPosition(pos);
}


int Path::totalCount_ = 0;


int Path::GetTotalCount()
{
    return totalCount_;
}
