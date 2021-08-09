#include "Observer.h"

vec4 Observer::getLocationAtAbsoluteTime(float t)
{
    return worldLine->getLocationAtAbsoluteTime(t);
}

vec4 Observer::getVelocityAtAbsoluteTime(float t)
{
    return worldLine->getVelocityAtAbsoluteTime(t);
}
