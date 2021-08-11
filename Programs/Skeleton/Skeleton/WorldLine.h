#pragma once

#include "framework.h"

#include "Entity.h"
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
	WorldLine(std::string _name = "", std::string _desc = ""): Entity(_name, _desc) {
	}

public:

	/*
	* The time measured by the absolute observer at the hyperplane,
	* where the "observer of this world line" is at a point,
	* where it measures tau proper time since it crossed the hyper plane of absolute observers t0 = 0;
	*/
	virtual float getAbsoluteTimeAtProperTime(float tau) = 0;

	/*
	* The proper time measured by the "line's observer" since it crossed the hyperplane of absolute observers t0 = 0,
	* and reached a hyperplane, where the absolute observer measures t time;
	*/
	virtual float getProperTimeAtAbsoluteTime(float t) = 0;

	/*
	* The space-time location on the world line according to the absolute observer, when the observer
	* of the line's proper time is tau since it crossed the hyperplane of absolute observers t = 0;
	*/
	virtual vec4 getLocationAtProperTime(float tau) = 0;
	/*
	* The velocity four-vector of the world line according to the absolute observer, when the "observer
	* of the line"'s proper time is tau since it crossed the hyperplane of absolute observers t = 0;
	*/
	virtual vec4 getVelocityAtProperTime(float tau) = 0;

	/*
	* Simultaneous hyperplane according the the line's observer,
	* when it measures tau proper time since it crossed the hyperplane of absolute observers t = 0
	*/
	virtual Hyperplane getSimultaneousHyperplaneAtProperTime(float tau) = 0;

	/*
	* The space-time location on the world line according to the absolute observer,
	* when the absolute observers time is t.
	*/
	virtual vec4 getLocationAtAbsoluteTime(float t) = 0;
	
	/*
	* The velocity four-vector of the world line according to the absolute observer,
	* when the absolute observers time is t.
	*/
	virtual vec4 getVelocityAtAbsoluteTime(float t) = 0;

	/*
	* Simultaneous hyperplane according the the line's observer,
	* when it crosses the hyperplane, where absolute observer measures t absolute time.
	*/
	virtual Hyperplane getSimultaneousHyperplaneAtAbsoluteTime(float t) = 0;

	/*
	* Returns intersection event in space-time between this world line and the intersectable hyperplane.
	*/
	virtual float intersectHyperplane(Hyperplane& plane) = 0;

	/*
	* Returns intersection event in space-time between this world line and the intersectable light cone.
	*/
	virtual float intersectLightCone(LightCone& cone) = 0;

	/*
	* Used, when a vertex is offseted from the center of an object.
	* Returned object must be deleted by caller!
	*/
	virtual WorldLine* getWorldLineWithOffset(vec3 offset) = 0;
};

class GeodeticLine : public WorldLine
{
	vec4 locationAtZeroT;	//When tau = 0
	vec4 fourVelocity;

public:

	/*
	* _velocity ... velocity of object in [m/m] according to the absolute observer.
	*/
	GeodeticLine(vec3 _posAtZeroT, vec3 _velocity, std::string _name = "", std::string _desc = "")
		: WorldLine(_name, _desc) {
		locationAtZeroT = vec4(_posAtZeroT.x, _posAtZeroT.y, _posAtZeroT.z, 0.0f);
		fourVelocity = RelPhysics::ToFourVelocity(_velocity);
	}
	GeodeticLine(vec4 _posAtZeroT, vec4 _velocity, std::string _name = "", std::string _desc = "")
		: locationAtZeroT(_posAtZeroT), fourVelocity(_velocity), WorldLine(_name, _desc) {
	}

	float getAbsoluteTimeAtProperTime(float tau);
	float getProperTimeAtAbsoluteTime(float t);
	vec4 getLocationAtProperTime(float tau);
	vec4 getVelocityAtProperTime(float tau);
	Hyperplane getSimultaneousHyperplaneAtProperTime(float tau);
	vec4 getLocationAtAbsoluteTime(float t);
	vec4 getVelocityAtAbsoluteTime(float t);
	Hyperplane getSimultaneousHyperplaneAtAbsoluteTime(float t);
	virtual float intersectHyperplane(Hyperplane& plane);
	virtual float intersectLightCone(LightCone& cone);
	virtual WorldLine* getWorldLineWithOffset(vec3 offset);


};
