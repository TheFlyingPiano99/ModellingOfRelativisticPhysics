#include "WorldLine.h"
#include <iostream>


//----------------------------------------------------
//GeodeticLine:

using namespace RelPhysics;

float GeodeticLine::getAbsoluteTimeAtProperTime(float tau)
{
    float gamma = 1.0f;
    try {
        gamma = lorentzFactor(relativeVelocity(absObserversFVelocity, fourVelocity));
    }
    catch (LightspeedExceededException e) {
        std::cerr << e.what() << std::endl;
    }
    return tau * gamma;
}

float GeodeticLine::getProperTimeAtAbsoluteTime(float t)
{
    float gamma = 1.0f;
    try {
        gamma = lorentzFactor(relativeVelocity(absObserversFVelocity, fourVelocity));
    }
    catch (LightspeedExceededException e) {
        std::cerr << e.what() << std::endl;
    }
    return t / gamma;
}

vec4 GeodeticLine::getLocationAtProperTime(float tau)
{
    return getLocationAtAbsoluteTime(getAbsoluteTimeAtProperTime(tau));
}

vec4 GeodeticLine::getVelocityAtProperTime(float tau)
{
    return fourVelocity;
}

Hyperplane GeodeticLine::getSimultaneousHyperplaneAtProperTime(float tau)
{
    return Hyperplane::simultaneousHyperplane(
        getLocationAtProperTime(tau),
        getVelocityAtProperTime(tau)
        );
}

vec4 GeodeticLine::getLocationAtAbsoluteTime(float t)
{
    return locationAtZeroT + fourVelocity * t;
}

vec4 GeodeticLine::getVelocityAtAbsoluteTime(float t)
{
    return fourVelocity;
}

Hyperplane GeodeticLine::getSimultaneousHyperplaneAtAbsoluteTime(float t)
{
    return Hyperplane::simultaneousHyperplane(
        getLocationAtAbsoluteTime(t),
        getVelocityAtAbsoluteTime(t)
    );
}

//------------------------------------------------------------------------