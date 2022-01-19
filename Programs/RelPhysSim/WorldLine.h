#pragma once

#include <fstream>

#include "Entity.h"
#include "Intersectable.h"
#include "Hyperplane.h"
#include "LightCone.h"
#include "RelPhysics.h"
#include "Exceptions.h"

/*
* World-line of an object / observer in space-time continuum.
* Abstract base class!
*/
class WorldLine : public Entity
{
protected:

	enum WorldLineType {
		geodetic,
		other
	} type;

	std::vector<vec4> vds4D;
	unsigned int noOfVds4D;

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
	* Used to select line when hovered over by mouse.
	*/
	float distanceBetweenRayAndDiagram(const Ray& ray, const RelTypes::ObserverProperties& observerProperties, const RelTypes::Settings& settings, vec4& closestLocation = vec4());

	/*
	* Intersects intersectable and returns time measered by absolute observer at intersection.
	*/
	float intersect(const Intersectable& intersectable);

	int getNoOfVds();

	std::vector<vec4>& getVds();

	const int getNoOfVds() const;

	const std::vector<vec4>& getVds() const;
	
	virtual void* createView() = 0;
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
	GeodeticLine(vec3 _posAtZeroT, vec3 _velocity, std::string _name = "", std::string _desc = "");

	GeodeticLine(vec4 _posAtZeroT, vec4 _velocity, std::string _name = "", std::string _desc = "");

	//WorldLine* getWorldLineWithOffset(vec3 offset);
	std::string genSaveString();

	/*
	* Returns the loaded object.
	*/
	static GeodeticLine* loadFromFile(std::ifstream& file);

	void draggedTo(vec4 location) override;
	vec4 getClosestLocation (
		const Ray& ray, 
		const RelTypes::ObserverProperties& observerProperties,
		const RelTypes::Settings& settings) override;
	void* createView() override;

	vec4 getLocationAtZeroT();

	vec4 getVelocity();
};

class CompositeLine : public WorldLine {
	std::vector<vec4> controlPoints;

	void genGeometry();
public:

	CompositeLine(std::vector<vec4>& points, std::string _name = "", std::string _desc = "");

	CompositeLine(vec3 _posAtZeroT, vec3 _velocity, std::string _name = "", std::string _desc = "");

	void draggedTo(vec4 location) override;


	// Inherited via WorldLine
	virtual std::string genSaveString() override;

	/*
	* Returns the loaded object.
	*/
	static CompositeLine* loadFromFile(std::ifstream& file);

	/*
	* 
	*/
	virtual vec4 getClosestLocation(
		const Ray& ray,
		const RelTypes::ObserverProperties& observerProperties,
		const RelTypes::Settings& settings) override;

	void setControlPoints(std::vector<vec4>& points);

	std::vector<vec4>& getControlPoints();

	int getClosestControlPointIndex(vec4 location);
	void* createView() override;

};

class SpiralLine : public WorldLine
{
	vec4 locationAtZeroT;	//When tau = 0
	vec4 fourVelocityAtZeroT;
	vec4 centerOfRotation;
	void genGeometry();

public:

	/*
	* _velocity ... velocity of object in [m/m] according to the absolute observer.
	*/
	SpiralLine(vec3 _posAtZeroT, vec3 _centerOfRotation, vec3 _velocity, std::string _name = "", std::string _desc = "");

	//WorldLine* getWorldLineWithOffset(vec3 offset);
	std::string genSaveString();

	/*
	* Returns the loaded object.
	*/
	static GeodeticLine* loadFromFile(std::ifstream& file);

	void draggedTo(vec4 location) override;
	vec4 getClosestLocation(
		const Ray& ray, 
		const RelTypes::ObserverProperties& observerProperties,
		const RelTypes::Settings& settings) override;
	void* createView() override;

	vec4 getLocationAtZeroT();

	vec4 getVelocity();
};
