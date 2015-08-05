#include "Urho3DAll.h"
#include "Utils.h"


float Distance(const Vector3& v1, const Vector3& v2)
{
    Vector3 diff = v1 - v2;
    return diff.Length();
}


