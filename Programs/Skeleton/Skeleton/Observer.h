#pragma once

#include "framework.h"

#include "Camera.h"
#include "Entity.h"
#include "WorldLine.h"
#include "Material.h"
#include "Caption.h"
#include "EnumTypes.h"
#include <map>
#include <memory>

class Observer : public Entity
{
	WorldLine* worldLine = NULL;
	float currentProperTime = 0.0f;
	int worldLineID = 0;
	std::shared_ptr<Caption*> diagramCaption;
	std::shared_ptr<Caption*> timerCaption;		// Displays the current proper time in diagram view.

public:

	Observer(WorldLine* _worldLine, std::string _name = "", std::string _desc = "")
		: Entity(_name, _desc), worldLine(_worldLine) {
		diagramCaption = Caption::createNormalCaption(vec3(0, 0, 0), name.c_str());
		timerCaption = Caption::createNormalCaption(vec3(0, 0, 0), "");
		(*diagramCaption)->setVisible(false);
		(*timerCaption)->setVisible(false);
	}

	~Observer() {
		delete worldLine;
		(*diagramCaption)->erease();
		(*timerCaption)->erease();		// static
	}

	/*
	* Returns spacetime location according to absolute observer when this observer measures "current proper time".
	* Interpolates between values, when transformation mode is changed.
	*/

	vec4 getLocation(const Settings& settings);

	/*
	* Returns velocity 4-vector according to absolute observer when this observer measures "current proper time".
	* Interpolates between values, when transformation mode is changed.
	*/
	vec4 getVelocity(const Settings& settings);

	/*
	* Return location in absolute observers frame, when this observer crosses absolute observers t = 0 hyperplane.
	* Interpolates between values, when transformation mode is changed.
	*/
	vec4 getLocationAtZero(const Settings& settings);

	/*
	* Return all necessary properties.
	* Interpolates between values, when transformation mode is changed.
	*/
	ObserverProperties getProperties(const Settings& settings);

	/*
	* Returns simultaneous hyperplane of this observer in absolute observers frame, where this observer measures currentProperTime.
	* Interpolates between values, when transformation mode is changed.
	*/
	Hyperplane* getHyperplane(const Settings& settings);

	/*
	* Returns light cone, where the observer measures currentProperTime.
	* Interpolates between values, when transformation mode is changed.
	*/
	LightCone* getLightCone(const Settings& settings);

	/*
	* Transform own position at current proper time to interpolated diagram position between proper frame and absolute frame.
	*/
	vec3 transformPosToDiagramSpace(vec4 pos4, const Settings& settings);

	/*
	* Transform hyperplane normal at current proper time to interpolated diagram normal between proper frame and absolute frame.
	*/
	vec3 transformNormalToDiagramSpace(vec4 normal4, const Settings& settings);

	/*
	* Transform light cone axis at current proper time to interpolated diagram axis between proper frame and absolute frame.
	*/
	vec3 transformConeAxisToDiagramSpace(const Settings& settings);

	void Draw(GPUProgram& gpuProgram, Camera& camera);
	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);
	void DrawHyperplane(GPUProgram& gpuProgram, Camera& camera, const Settings& settings);
	void DrawLightCone(GPUProgram& gpuProgram, Camera& camera, const Settings& settings);
	void DrawNode(GPUProgram& gpuProgram, Camera& camera, const Settings& settings);
	
	/*
	* Draw intersectable and captions.
	*/
	void DrawExtras(GPUProgram& gpuProgram, Camera& camera, const ObserverProperties& observerProperties, const Settings& settings);

	/*
	* Receives time in absolute frame and sets proper time accordingly.
	*/
	void setCurrentTimeAtAbsoluteTime(float t, const Settings& settings);

	/*
	* Returns time in absolute frame, where observers frame measures currentProperTime.
	*/
	float getAbsoluteTimeAtCurrentTime(const Settings& settings);

	/*
	* Receives delta time in proper frame if it does not sinc below zero.
	* Returns actually applied delta tau.
	*/
	float changeTimeByDelta(float deltaTau, const Settings& settings);

	void syncCamera(Camera* camera, const Settings& settings);
	
	/*
	* Sync proper time of this observer so, that it matches the time measured by ths observer, when it's world line crosses the given observers simultaneous hyperplane.
	*/
	void syncTimeToObserversSimultaneity(Observer& observer, const Settings& settings);

	void loadOnGPU(GPUProgram& gpuProgram, const Settings& settings);

	std::string genSaveString();

	/*
	* Returns the loaded object.
	*/
	static Observer* loadFromFile(std::ifstream& file);

	void setWorldLineID(const int id) {
		worldLineID = id;
	}

	int getWorldLineID() {
		return worldLineID;
	}

	WorldLine* getWorldLine() {
		return worldLine;
	}

	/*
	* Must be called after loading!
	*/
	void setWorldLine(std::map<int, WorldLine*>& worldLines) {
		worldLine = worldLines.at(worldLineID);
	}

	/*
	* Mouse is hovered above the image of the observer.
	*/
	void hover();
};

