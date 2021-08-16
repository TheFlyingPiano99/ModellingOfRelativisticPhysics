#include "WorldLine.h"
#include <iostream>

//----------------------------------------------------
//GeodeticLine:

using namespace RelPhysics;

inline void GeodeticLine::genGeometry() {
    vds.push_back(vec3(locationAtZeroT.x, locationAtZeroT.y, locationAtZeroT.w));

    for (int i = 0; i < 10; i++) {
        vec4 pos = getLocationAtAbsoluteTime(i * 10);
        vds.push_back(vec3(pos.x, pos.y, pos.w));
    }
    noOfVds = vds.size();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, noOfVds * sizeof(vec3), &vds[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

float GeodeticLine::getAbsoluteTimeAtProperTime(float tau)
{
    float gamma = 1.0f;
    try {
        gamma = lorentzFactor(relativeVelocity(absObservers4Velocity, fourVelocity));
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
        gamma = lorentzFactor(relativeVelocity(absObservers4Velocity, fourVelocity));
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
    return locationAtZeroT + fourVelocity / fourVelocity.w * t;
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

/*
* Returns the absolute time spent, where the world line intersects the hyperplane.
*/
float GeodeticLine::intersectHyperplane(Hyperplane& plane)
{
    return (dot(plane.getLocation() - locationAtZeroT, plane.getNormal())) / dot(fourVelocity, plane.getNormal()) * fourVelocity.w;
}

float GeodeticLine::intersectLightCone(LightCone& cone)
{
    float a, b, c, t;
    a = LorentzianProduct(fourVelocity, fourVelocity);
    b = 2 * (
        (locationAtZeroT.x * fourVelocity.x
            - fourVelocity.x * cone.getObserver().x)
        + (locationAtZeroT.y * fourVelocity.y
            - fourVelocity.y * cone.getObserver().y)
        + (locationAtZeroT.z * fourVelocity.z
            - fourVelocity.z * cone.getObserver().z)
        - (locationAtZeroT.w * fourVelocity.w
            - fourVelocity.w * cone.getObserver().w)
        );

    c = powf(cone.getObserver().x - locationAtZeroT.x, 2)
        + powf(cone.getObserver().y - locationAtZeroT.y, 2)
        + powf(cone.getObserver().z - locationAtZeroT.z, 2)
        - powf(cone.getObserver().w - locationAtZeroT.w, 2);

    int noOfSolutions;
    vec2 solutions = solveQuadraticFunction(a, b, c, noOfSolutions);
    std::cout << "Number of solutions = " << noOfSolutions << std::endl;
    if (noOfSolutions > 0) {
        t = solutions.x;
    }
    else {
        throw DoesNotIntersectException();
    }
    return t * fourVelocity.w;
}

WorldLine* GeodeticLine::getWorldLineWithOffset(vec3 offset)
{
    return new GeodeticLine(
        locationAtZeroT + vec4(offset.x, offset.y, offset.z, 0),
        fourVelocity,
        name,
        description);
}

void GeodeticLine::loadOnGPU(GPUProgram& gpuProgram)
{
    //gpuProgram.setUniform(type, "worldLineType");
    gpuProgram.setUniform(locationAtZeroT, "subjectsStartPos");
    gpuProgram.setUniform(fourVelocity, "subjectsVelocity");
}

void GeodeticLine::Draw()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
}

//------------------------------------------------------------------------