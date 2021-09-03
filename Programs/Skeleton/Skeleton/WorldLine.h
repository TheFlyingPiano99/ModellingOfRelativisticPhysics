#pragma once

#include "framework.h"

#include "Entity.h"
#include "Intersectable.h"
#include "Hyperplane.h"
#include "LightCone.h"
#include "RelPhysics.h"
#include "Exceptions.h"
#include <fstream>
#include "Ray.h"

/*
* World-line of an object / observer in space-time continuum.
* Abstract base class!
*/
class WorldLine : public Entity
{
protected:
	unsigned int vao, vbo;

	//New part:
	std::vector<vec4> vds4D;
	unsigned int noOfVds4D;

	enum WorldLineType {
		geodetic,
		other
	} type;



	//Functions:--------------------------------------------------------

	WorldLine(std::string _name = "", std::string _desc = "");

	virtual void genGeometry() = 0;

public:

	~WorldLine();


	/*
	* The time measured by the absolute observer at the hyperplane,
	* where the "observer of this world line" is at a point,
	* where it measures tau proper time since it crossed the hyper plane of absolute observers t0 = 0;
	*/
	float getAbsoluteTimeAtProperTime(float tau);

	/*
	* The proper time measured by the "line's observer" since it crossed the hyperplane of absolute observers t0 = 0,
	* and reached a hyperplane, where the absolute observer measures t time;
	*/
	float getProperTimeAtAbsoluteTime(float t);

	/*
	* The space-time location on the world line according to the absolute observer, when the observer
	* of the line's proper time is tau since it crossed the hyperplane of absolute observers t = 0;
	*/
	vec4 getLocationAtProperTime(float tau);
	/*
	* The velocity four-vector of the world line according to the absolute observer, when the "observer
	* of the line"'s proper time is tau since it crossed the hyperplane of absolute observers t = 0;
	*/
	vec4 getVelocityAtProperTime(float tau);

	/*
	* Simultaneous hyperplane according the the line's observer,
	* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
	*/
	Hyperplane* getSimultaneousHyperplaneAtProperTime(float tau);

	/*
	* Light cone according the the line's observer,
	* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
	*/
	LightCone* getLigtConeAtProperTime(float tau);

	/*
	* The space-time location on the world line according to the absolute observer,
	* when the absolute observers time is t.
	*/
	vec4 getLocationAtAbsoluteTime(float t);

	/*
	* The velocity four-vector of the world line according to the absolute observer,
	* when the absolute observers time is t.
	*/
	vec4 getVelocityAtAbsoluteTime(float t);

	/*
	* Simultaneous hyperplane according the the line's observer,
	* when it crosses the hyperplane, where absolute observer measures t absolute time.
	*/
	Hyperplane* getSimultaneousHyperplaneAtAbsoluteTime(float t);

	/*
	* Light cone according the the line's observer,
	* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
	*/
	LightCone* getLigtConeAtAbsoluteTime(float t);

	/*
	* Returns the absolute time spent, where the world line intersects the hyperplane.
	*/
	float intersectHyperplane(const Hyperplane& plane);

	/*
	* Returns the absolute time spent, where the world line intersects the light cone.
	*/
	float intersectLightCone(const LightCone& cone);

	/*
	* Used, when a vertex is offseted from the center of an object.
	* Returned object must be deleted by caller!
	*/
	//virtual WorldLine* getWorldLineWithOffset(vec3 offset) = 0;

	void loadOnGPU(GPUProgram& gpuProgram);

	/*
	* Draw in diagram view.
	*/
	void DrawDiagram();

	float distanceBetweenRayAndDiagram(const Ray& ray, const ObserverProperties& observerProperties, const Settings& settings);

	/*
	* Intersects intersectable and returns time measered by absolute observer at intersection.
	*/
	float intersect(const Intersectable& intersectable);
};

class GeodeticLine : public WorldLine
{
	vec4 locationAtZeroT;	//When tau = 0
	vec4 fourVelocity;

	void genGeometry();
public:

	/*
	* _velocity ... velocity of object in [m/m] according to the absolute observer.
	*/
	GeodeticLine(vec3 _posAtZeroT, vec3 _velocity, std::string _name = "", std::string _desc = "")
		: WorldLine(_name, _desc) {
		type = WorldLineType::geodetic;
		locationAtZeroT = vec4(_posAtZeroT.x, _posAtZeroT.y, _posAtZeroT.z, 0.0f);
		fourVelocity = RelPhysics::ToFourVelocity(_velocity);
		noOfVds4D = 2;
		genGeometry();
	}

	GeodeticLine(vec4 _posAtZeroT, vec4 _velocity, std::string _name = "", std::string _desc = "")
		: locationAtZeroT(_posAtZeroT), fourVelocity(_velocity), WorldLine(_name, _desc) {
		type = WorldLineType::geodetic;
		noOfVds4D = 2;
		genGeometry();
	}

	//WorldLine* getWorldLineWithOffset(vec3 offset);
	std::string genSaveString();

	/*
	* Returns the loaded object.
	*/
	static GeodeticLine* loadFromFile(std::ifstream& file);
};

class CompositeLine : public WorldLine {
	std::vector<WorldLine*> parts;


public:

	CompositeLine(vec3 _posAtZeroT, vec3 _velocity, std::string _name = "", std::string _desc = "")
		: WorldLine(_name, _desc) {}

	//Todo

};