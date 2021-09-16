#include "WorldLine.h"
#include "WorldLineView.h"
#include <iostream>
#include "StringOperations.h"

//----------------------------------------------------
//GeodeticLine:

using namespace RelPhysics;

inline void GeodeticLine::genGeometry() {
    vds4D.resize(shaderWorldLineResolution);     // Size given in shader.
    for (int i = 0; i < noOfVds4D; i++) {
        vec4 pos = locationAtZeroT + fourVelocity / fourVelocity.w * (i - noOfVds4D / 2.0f) * 50;
        vds4D[i] = pos;
    }
}


/*
WorldLine* GeodeticLine::getWorldLineWithOffset(vec3 offset)
{
    return new GeodeticLine(
        locationAtZeroT + vec4(offset.x, offset.y, offset.z, 0),
        fourVelocity,
        name,
        description);
}
*/

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

WorldLine::WorldLine(std::string _name, std::string _desc) : Entity(_name, _desc) {

}

WorldLine::~WorldLine() {

}

/*
* The time measured by the absolute observer at the hyperplane,
* where the "observer of this world line" is at a point,
* where it measures tau proper time since it crossed the hyper plane of absolute observers t0 = 0;
*/

float WorldLine::getAbsoluteTimeAtProperTime(float tau)
{
    if (tau == 0.0f) {
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

/*
* The proper time measured by the "line's observer" since it crossed the hyperplane of absolute observers t0 = 0,
* and reached a hyperplane, where the absolute observer measures t time;
*/

float WorldLine::getProperTimeAtAbsoluteTime(float t)
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

/*
* The space-time location on the world line according to the absolute observer, when the observer
* of the line's proper time is tau since it crossed the hyperplane of absolute observers t = 0;
*/

vec4 WorldLine::getLocationAtProperTime(float tau)
{
    return getLocationAtAbsoluteTime(getAbsoluteTimeAtProperTime(tau));
}

/*
* The velocity four-vector of the world line according to the absolute observer, when the "observer
* of the line"'s proper time is tau since it crossed the hyperplane of absolute observers t = 0;
*/

vec4 WorldLine::getVelocityAtProperTime(float tau)
{
    return getVelocityAtAbsoluteTime(getAbsoluteTimeAtProperTime(tau));
}

/*
* Simultaneous hyperplane according the the line's observer,
* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
*/

Hyperplane* WorldLine::getSimultaneousHyperplaneAtProperTime(float tau)
{
    return Hyperplane::simultaneousHyperplane(
        getLocationAtProperTime(tau),
        getVelocityAtProperTime(tau)
    );
}

/*
* Light cone according the the line's observer,
* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
*/

LightCone* WorldLine::getLigtConeAtProperTime(float tau)
{
    return new LightCone(getLocationAtProperTime(tau));
}

/*
* The space-time location on the world line according to the absolute observer,
* when the absolute observers time is t.
*/

vec4 WorldLine::getLocationAtAbsoluteTime(float t)
{
    vec4 tangentVelocity;
    vec4 offsettedStartPos;
    for (int i = 0; i < noOfVds4D - 1; i++) {
        if (
            (i == 0 && t < vds4D[1].w)                                          // First section
            || (i < noOfVds4D - 2 && t < vds4D[i + 1].w && t >= vds4D[i].w)     // Middle section
            || (i == noOfVds4D - 2 && t >= vds4D[i].w)                          // Last section
            ) {
            tangentVelocity = tangentFourVelocity(vds4D[i], vds4D[i + 1]);
            offsettedStartPos = vds4D[i] - tangentVelocity / tangentVelocity.w * vds4D[i].w;
            break;
        }
    }
    return offsettedStartPos + tangentVelocity / tangentVelocity.w * t;
}

/*
* The velocity four-vector of the world line according to the absolute observer,
* when the absolute observers time is t.
*/

vec4 WorldLine::getVelocityAtAbsoluteTime(float t)
{
    vec4 tangentVelocity;
    for (int i = 0; i < noOfVds4D - 1; i++) {
        if (
            (i == 0 && t < vds4D[1].w)                                          // First section
            || (i < noOfVds4D - 2 && t < vds4D[i + 1].w && t >= vds4D[i].w)     // Middle section
            || (i == noOfVds4D - 2 && t >= vds4D[i].w)                          // Last section
            ) {
            tangentVelocity = tangentFourVelocity(vds4D[i], vds4D[i + 1]);
        }
    }
    return tangentVelocity;
}

/*
* Simultaneous hyperplane according the the line's observer,
* when it crosses the hyperplane, where absolute observer measures t absolute time.
*/

Hyperplane* WorldLine::getSimultaneousHyperplaneAtAbsoluteTime(float t)
{
    return Hyperplane::simultaneousHyperplane(
        getLocationAtAbsoluteTime(t),
        getVelocityAtAbsoluteTime(t)
    );
}

/*
* Light cone according the the line's observer,
* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
*/

LightCone* WorldLine::getLigtConeAtAbsoluteTime(float t) {
    return new LightCone(getLocationAtAbsoluteTime(t));
}

/*
* Returns the absolute time spent, where the world line intersects the hyperplane.
*/

float WorldLine::intersectHyperplane(const Hyperplane& plane)
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

/*
* Returns the absolute time spent, where the world line intersects the light cone.
*/

float WorldLine::intersectLightCone(const LightCone& cone)
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

        c = powf(cone.getLocation().x - offsettedStartPos.x, 2)
            + powf(cone.getLocation().y - offsettedStartPos.y, 2)
            + powf(cone.getLocation().z - offsettedStartPos.z, 2)
            - powf(cone.getLocation().w - offsettedStartPos.w, 2);

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

void WorldLine::loadOnGPU(GPUProgram& gpuProgram)
{
    gpuProgram.setUniform(vds4D, "worldLineNodes");
    gpuProgram.setUniform((int)noOfVds4D, "noOfWorldLineNodes");
}

float WorldLine::distanceBetweenRayAndDiagram(const Ray& ray, const ObserverProperties& observerProperties, const Settings& settings, vec4& closestLocation)
{
    float distance = -1;
    closestLocation = vds4D[0];
    //vec3 closestPoint;      // Todo
    for (int i = 0; i < noOfVds4D - 1; i++) {
        vec4 pos1 = vds4D[i];
        vec4 pos2 = vds4D[i + 1];
        if (settings.transformToProperFrame.get()) {
            // !!! Eucleadian is not correct !!!
            pos1 = (settings.doLorentz.get()) ? lorentzTransformation(pos1 - observerProperties.locationAtZero, To3DVelocity(observerProperties.velocity))
                : galileanTransformation(pos1 - observerProperties.locationAtZero, To3DVelocity(observerProperties.velocity));
            pos2 = (settings.doLorentz.get()) ? lorentzTransformation(pos2 - observerProperties.locationAtZero, To3DVelocity(observerProperties.velocity))
                : galileanTransformation(pos2 - observerProperties.locationAtZero, To3DVelocity(observerProperties.velocity));
        }
        vec4 tangentVelocity = tangentFourVelocity(pos1, pos2);
        vec4 offsettedStartPos = pos1 - tangentVelocity / tangentVelocity.w * pos1.w;
        vec3 diagramStartPos = vec3(offsettedStartPos[settings.diagramX], offsettedStartPos[settings.diagramY], offsettedStartPos[settings.diagramZ]);
        vec3 diagramDir = normalize(vec3(tangentVelocity[settings.diagramX], tangentVelocity[settings.diagramY], tangentVelocity[settings.diagramZ]));
        float temp = abs(dot(ray.pos - diagramStartPos, cross(diagramDir, ray.dir)));
        //vec3 cn = normalize(cross(diagramDir, ray.dir));
        //vec3 projected = dot(diagramPos - ray.pos, ray.dir) * ray.dir;
        //vec3 rejected = diagramPos - ray.pos - projected - dot(diagramPos - ray.pos, cn) * cn;
        //closestPoint = diagramPos - diagramDir * normalize(rejected) / dot(diagramDir, normalize(rejected));
        vec3 endPoint1 = vec3(pos1[settings.diagramX], pos1[settings.diagramY], pos1[settings.diagramZ]);
        vec3 endPoint2 = vec3(pos2[settings.diagramX], pos2[settings.diagramY], pos2[settings.diagramZ]);
        if (/*dot(endPoint2 - endPoint1, closestPoint - endPoint1) > 0.0f && dot(endPoint1 - endPoint2, closestPoint - endPoint2) > 0.0f && */
            (distance == -1 || temp < diagram)) {
            distance = temp;
            closestLocation = pos1;
        }
    }
    return distance;
}

/*
*
*/

float WorldLine::intersect(const Intersectable& intersectable) {
    float t;
    if (intersectable.getType() == IntersectionMode::lightCone) {
        t = intersectLightCone(reinterpret_cast<const LightCone&>(intersectable));
    }
    else if (intersectable.getType() == IntersectionMode::hyperplane) {
        t = intersectHyperplane(reinterpret_cast<const Hyperplane&>(intersectable));
    }
    return t;
}


void GeodeticLine::draggedTo(vec4 location)
{
    if (length(location - locationAtZeroT) < 2.0f) {
        locationAtZeroT = location;
        genGeometry();
        locationAtZeroT.w = 0.0f;
        return;
    }
    vec4 temp = normalize(location - locationAtZeroT) * RelPhysics::speedOfLight;
    if (temp.w < 0) {
        temp = temp * -1.0f;
    }
    if (length(RelPhysics::To3DVelocity(temp)) > RelPhysics::speedOfLight * 0.99f) {   // Don't set if speed of light exceeded!
        return;
    }
    fourVelocity = temp;
    genGeometry();
}

vec4 GeodeticLine::getClosestLocation(const Ray& ray, const ObserverProperties& observerProperties, const Settings& settings)
{
    return locationAtZeroT;
    //vec4 closestLocation;
    //distanceBetweenRayAndDiagram(ray, observerProperties, settings, closestLocation);
   //return closestLocation;
}

void* GeodeticLine::createView()
{
    return new GeodeticLineView(this);
}

void CompositeLine::genGeometry()
{
    vds4D.resize(shaderWorldLineResolution);     // Size given in shader.
    vec4 velocity;
    int countVDS = 0;
    velocity = RelPhysics::tangentFourVelocity(controlPoints[0], controlPoints[1]);
    vds4D[countVDS++] = controlPoints[0] + velocity * -50.0f;
    for (int i = 0; i < controlPoints.size(); i++) {
        vds4D[countVDS++] = controlPoints[i];
    }
    velocity = RelPhysics::tangentFourVelocity(controlPoints[controlPoints.size() - 2], controlPoints[controlPoints.size() - 1]);
    vds4D[countVDS++] = controlPoints[controlPoints.size() - 1] + velocity * 50.0f;
    noOfVds4D = countVDS;
}

void CompositeLine::draggedTo(vec4 location)
{
    int idx = getClosestControlPointIndex(location);
    vec4 prevVelocity = vec4(0, 0, 0, 1);
    vec4 follVelocity = vec4(0, 0, 0, 1);
    if (idx > 0) {
        prevVelocity = RelPhysics::tangentFourVelocity(controlPoints[idx - 1], location);
        if (length(RelPhysics::To3DVelocity(prevVelocity)) > RelPhysics::speedOfLight * 0.99f
            ||
            location.w < controlPoints[idx - 1].w
            ) {
            return;
        }
    }
    if (idx < controlPoints.size() - 1) {
        follVelocity = RelPhysics::tangentFourVelocity(location, controlPoints[idx + 1]);
        if (length(RelPhysics::To3DVelocity(follVelocity)) > RelPhysics::speedOfLight * 0.99f
            ||
            location.w > controlPoints[idx + 1].w
            ) {
            return;
        }
    }

    controlPoints[idx] = location;

    genGeometry();

}

std::string CompositeLine::genSaveString()
{
    std::string str(
        "CompositeLine\n"
        "ID " + std::to_string(getID()) + "\n"
        "name " + name + "\n"
        "description " + description + "\n"
        "noOfControlPoints " + std::to_string(controlPoints.size()) + "\n"
        "controlPoints\n"
    );
    for (const vec4& cp : controlPoints) {
        str.append(std::to_string(cp.x) + " " + std::to_string(cp.y) + " " + std::to_string(cp.z) + " " + std::to_string(cp.w) + "\n");
    }
    str.append(
        "!controlPoints\n"
        "!CompositeLine\n");
    return str;
}

CompositeLine* CompositeLine::loadFromFile(std::ifstream& file)
{
    int _ID;
    std::string _name;
    std::string _description;
    std::vector<vec4> controlPoints;
    bool readControlPoints = false;
    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> words = split(line, ' ');
        if (words.empty()) {									// Empty line
            continue;
        }
        else if (words.at(0).at(0) == '#') {					// Comment
            continue;
        }
        else if (words.at(0).compare("!CompositeLine") == 0) {	// End of declaration
            CompositeLine* retVal = new CompositeLine(controlPoints, _name, _description);
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
        else if (words.at(0).compare("controlPoints") == 0) {         // controlPoints
            readControlPoints = true;
        }
        else if (words.at(0).compare("!controlPoints") == 0) {         // controlPoints
            readControlPoints = false;
        }
        else if (readControlPoints) {
            controlPoints.push_back(vec4(stof(words[0]), stof(words[1]), stof(words[2]), stof(words[3])));
        }
    }
    return nullptr;
}

vec4 CompositeLine::getClosestLocation(const Ray& ray, const ObserverProperties& observerProperties, const Settings& settings)
{
    return controlPoints[0];
}

int CompositeLine::getClosestControlPointIndex(vec4 location)
{
    int idx = -1;
    float dist = -1;
    for (int i = 0; i < controlPoints.size(); i++) {
        float temp = length(controlPoints[i] - location);
        if (dist < 0.0f || temp < dist) {
            dist = temp;
            idx = i;
        }
    }
    return idx;
}

void* CompositeLine::createView()
{
    return new CompositeLineView(this);
}

