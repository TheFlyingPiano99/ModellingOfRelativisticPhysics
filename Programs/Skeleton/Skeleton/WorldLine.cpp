#include "WorldLine.h"
#include <iostream>
#include "StringOperations.h"

//----------------------------------------------------
//GeodeticLine:

using namespace RelPhysics;

inline void GeodeticLine::genGeometry() {
    vds4D.resize(1000);     // Size give in shader.
    for (int i = 0; i < 2; i++) {
        vec4 pos = getLocationAtAbsoluteTime((i - 50) * 10);
        vds.push_back(vec3(pos.x, pos.y, pos.w));
        vds4D[i] = pos;
    }
    noOfVds = vds.size();
    noOfVds4D = noOfVds;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, noOfVds * sizeof(vec3), &vds[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}


float GeodeticLine::getAbsoluteTimeAtProperTime(float tau)
{
    if (tau == 0.0f) {
        return 0.0f;
    }

    // Find the "nearest to zero" absolute time coordinate:
    int startIdx = 0;
    float distToZero = (vds4D[0].w >= 0)? vds4D[0].w : -1;
    if (distToZero > 0 || distToZero == -1) {
        for (int i = 1; i < noOfVds4D - 1; i++) {       // Linear search <- should be changed to binary search
            float w = vds4D[i].w;
            if (w >= 0.0f) {
                if (w == 0.0f) {
                    startIdx = i;
                    distToZero = 0.0f;
                    break;
                }
                if (w < distToZero || distToZero == -1) {
                    distToZero = w;
                    startIdx = i;
                }
            }
        }
    }
    
    float spentT = distToZero;
    float spentTau = distToZero / lorentzFactor(relativeVelocity(absObservers4Velocity, tangentFourVelocity(vds4D[startIdx], vds4D[startIdx + 1])));
    for (int i = startIdx; i < noOfVds4D - 1; i++) {
        float gamma = 1.0f;
        try {
            gamma = lorentzFactor(relativeVelocity(absObservers4Velocity, tangentFourVelocity(vds4D[i + 1], vds4D[i])));
        }
        catch (LightspeedExceededException e) {
            std::cerr << e.what() << std::endl;
        }
        if (i == noOfVds4D - 2 || (spentTau + (vds4D[i + 1].w - vds4D[i].w) / gamma) >= tau) {                                      // Tau is reached in this section!
            float leftOverTau = tau - spentTau;
            return spentT + gamma * leftOverTau;        
        }
        spentT += vds4D[i + 1].w - vds4D[i].w;
        spentTau += (vds4D[i + 1].w - vds4D[i].w) / gamma;
    }
}

float GeodeticLine::getProperTimeAtAbsoluteTime(float t)
{
    if (t == 0.0f) {
        return 0.0f;
    }

    // Find the "nearest to zero" absolute time coordinate:
    int startIdx = 0;
    float distToZero = (vds4D[0].w >= 0) ? vds4D[0].w : -1;
    if (distToZero > 0 || distToZero == -1) {
        for (int i = 1; i < noOfVds4D - 1; i++) {       // Linear search <- should be changed to binary search
            float w = vds4D[i].w;
            if (w >= 0.0f) {
                if (w == 0.0f) {
                    startIdx = i;
                    distToZero = 0.0f;
                    break;
                }
                if (w < distToZero || distToZero == -1) {
                    distToZero = w;
                    startIdx = i;
                }
            }
        }
    }

    float spentT = distToZero;
    float spentTau = distToZero / lorentzFactor(relativeVelocity(absObservers4Velocity, tangentFourVelocity(vds4D[startIdx], vds4D[startIdx + 1])));
    for (int i = startIdx; i < noOfVds4D - 1; i++) {
        float gamma = 1.0f;
        try {
            gamma = lorentzFactor(relativeVelocity(absObservers4Velocity, tangentFourVelocity(vds4D[i], vds4D[i + 1])));
        }
        catch (LightspeedExceededException e) {
            std::cerr << e.what() << std::endl;
        }
        if (i == noOfVds4D - 2 || (spentT + (vds4D[i + 1].w - vds4D[i].w)) >= t) {                                      // Tau is reached in this section!
            float leftOverT = t - spentT;
            return spentTau + leftOverT / gamma;
        }
        spentT += vds4D[i + 1].w - vds4D[i].w;
        spentTau += (vds4D[i + 1].w - vds4D[i].w) / gamma;
    }
}

vec4 GeodeticLine::getLocationAtProperTime(float tau)
{
    return getLocationAtAbsoluteTime(getAbsoluteTimeAtProperTime(tau));
}

vec4 GeodeticLine::getVelocityAtProperTime(float tau)
{
    return fourVelocity;
}

Hyperplane* GeodeticLine::getSimultaneousHyperplaneAtProperTime(float tau)
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

Hyperplane* GeodeticLine::getSimultaneousHyperplaneAtAbsoluteTime(float t)
{
    return Hyperplane::simultaneousHyperplane(
        getLocationAtAbsoluteTime(t),
        getVelocityAtAbsoluteTime(t)
    );
}

LightCone* GeodeticLine::getLigtConeAtAbsoluteTime(float t) {
    return new LightCone(getLocationAtAbsoluteTime(t));
}

float GeodeticLine::intersectHyperplane(Hyperplane& plane)
{
    float t = 0;
    for (int i = 0; i < noOfVds4D - 1; i++) {
        vec4 tangentVelocity = tangentFourVelocity(vds4D[i], vds4D[i + 1]);
        vec4 offsettedStartPos = vds4D[i] - tangentVelocity / tangentVelocity.w * vds4D[i].w;
        t = (dot(plane.getLocation() - offsettedStartPos, plane.getNormal())) / dot(tangentVelocity, plane.getNormal()) * tangentVelocity.w;
        if (
            (i == 0 && t < vds4D[1].w)                                          // First section
            || (i < noOfVds4D - 2 && t < vds4D[i + 1].w && t >= vds4D[i].w)     // Middle section
            || (i == noOfVds4D - 2 && t >= vds4D[i].w)                          // Last section
            ) {
            break;
        }
    }

    return t;
}

float GeodeticLine::intersectLightCone(LightCone& cone)
{
    float t = 0;
    for (int i = 0; i < noOfVds4D - 1; i++) {
        vec4 tangentVelocity = tangentFourVelocity(vds4D[i], vds4D[i + 1]);
        vec4 offsettedStartPos = vds4D[i] - tangentVelocity / tangentVelocity.w * vds4D[i].w;
        float a, b, c;
        a = LorentzianProduct(tangentVelocity, tangentVelocity);
        b = 2 * (
            (offsettedStartPos.x * tangentVelocity.x
                - tangentVelocity.x * cone.getLocation().x)
            + (offsettedStartPos.y * tangentVelocity.y
                - tangentVelocity.y * cone.getLocation().y)
            + (offsettedStartPos.z * tangentVelocity.z
                - tangentVelocity.z * cone.getLocation().z)
            - (offsettedStartPos.w * tangentVelocity.w
                - tangentVelocity.w * cone.getLocation().w)
            );

        c = powf(cone.getLocation().x - locationAtZeroT.x, 2)
            + powf(cone.getLocation().y - locationAtZeroT.y, 2)
            + powf(cone.getLocation().z - locationAtZeroT.z, 2)
            - powf(cone.getLocation().w - locationAtZeroT.w, 2);

        int noOfSolutions;
        vec2 solutions = solveQuadraticFunction(a, b, c, noOfSolutions);
        if (noOfSolutions > 0) {
            t = solutions.x * tangentVelocity.w;
        }
        else {
            throw DoesNotIntersectException();
        }
        if (
            (i == 0 && t < vds4D[1].w)                                          // First section
            || (i < noOfVds4D - 2 && t < vds4D[i + 1].w && t >= vds4D[i].w)     // Middle section
            || (i == noOfVds4D - 2 && t >= vds4D[i].w)                          // Last section
            ) {
            break;
        }
    }
    return t;
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

    //New part:
    gpuProgram.setUniform(vds4D, "worldLineNodes");
    gpuProgram.setUniform((int)noOfVds4D, "noOfWorldLineNodes");

}

void GeodeticLine::Draw()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glLineWidth(3);
    glDrawArrays(GL_LINE_STRIP, 0, noOfVds);
}

LightCone* GeodeticLine::getLigtConeAtProperTime(float tau)
{
    return new LightCone(getLocationAtProperTime(tau));
}

float GeodeticLine::distanceBetweenRayAndDiagram(const Ray& ray)
{
    vec3 wlPos = vec3(locationAtZeroT.x, locationAtZeroT.y, locationAtZeroT.w);
    vec3 wlDir = normalize(vec3(fourVelocity.x, fourVelocity.y, fourVelocity.w));
    return abs(dot(ray.pos - wlPos, cross(wlDir, ray.dir)));
}

GeodeticLine* GeodeticLine::loadFromFile(std::ifstream& file)
{
    int _ID;
    std::string _name;
    std::string _description;
    vec4 _locationAtZeroT;
    vec4 _fourVelocity;

    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> words = split(line, ' ');
        if (words.empty()) {									// Empty line
            continue;
        }
        else if (words.at(0).at(0) == '#') {					// Comment
            continue;
        }
        else if (words.at(0).compare("!GeodeticLine") == 0) {	// End of declaration
            GeodeticLine* retVal = new GeodeticLine(_locationAtZeroT, _fourVelocity, _name, _description);
            retVal->setID(_ID);
            return retVal;
        }
        else if (words.at(0).compare("ID") == 0) {              // ID
            _ID = std::stoi(words.at(1));
        }
        else if (words.at(0).compare("name") == 0) {              // name
            _name = join(words, 1);
        }
        else if (words.at(0).compare("description") == 0) {         // description
            _description = join(words, 1);
        }
        else if (words.at(0).compare("locationAtZeroT") == 0) {         // locationAtZeroT
            _locationAtZeroT = vec4(stof(words[1]), stof(words[2]), stof(words[3]), stof(words[4]));
        }
        else if (words.at(0).compare("fourVelocity") == 0) {         // fourVelocity
            _fourVelocity = vec4(stof(words[1]), stof(words[2]), stof(words[3]), stof(words[4]));
        }
    }
    return nullptr;
}

std::string GeodeticLine::genSaveString()
{
    std::string str(
        "GeodeticLine\n"
        "ID " + std::to_string(getID()) + "\n"
        "name " + name + "\n"
        "description " + description + "\n"
        "locationAtZeroT " + std::to_string(locationAtZeroT.x) + " " + std::to_string(locationAtZeroT.y) + " " + std::to_string(locationAtZeroT.z) + " " + std::to_string(locationAtZeroT.w) + "\n"
        "fourVelocity " + std::to_string(fourVelocity.x) + " " + std::to_string(fourVelocity.y) + " " + std::to_string(fourVelocity.z) + " " + std::to_string(fourVelocity.w) + "\n"
        "!GeodeticLine\n"
    );
    return str;
}

//------------------------------------------------------------------------