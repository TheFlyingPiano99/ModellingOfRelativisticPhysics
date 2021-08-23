#pragma once

#include "framework.h"

#include "Camera.h"
#include "Entity.h"
#include "WorldLine.h"
#include "Material.h"
#include <map>


class Observer : public Entity
{
	WorldLine* worldLine = NULL;
	float currentProperTime = 0.0f;
	int worldLineID = 0;

public:

	Observer(WorldLine* _worldLine, std::string _name = "", std::string _desc = "")
		: Entity(_name, _desc), worldLine(_worldLine) {
	}

	~Observer() {
		delete worldLine;
	}

	vec4 getLocation();

	vec4 getVelocity();

	vec4 getStartPos();

	Hyperplane getHyperplane();

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);
	void DrawHyperplane(GPUProgram& gpuProgram, Camera& camera);
	void DrawLightCone(GPUProgram& gpuProgram, Camera& camera);
	void DrawNode(GPUProgram& gpuProgram, Camera& camera);
	void DrawExtras(GPUProgram& gpuProgram, Camera& camera);

	/*
	* Receives time in absolute frame and sets proper time accordingly.
	*/
	void setCurrentTimeAtAbsoluteTime(float t);

	/*
	* Returns time in absolute frame, where observers frame measures currentProperTime.
	*/
	float getAbsoluteTimeAtCurrentTime();

	/*
	* Receives delta time in proper frame.
	*/
	void increaseTimeByDelta(float deltaTau);

	void syncCamera(Camera* camera);
	void syncTimeToObserversSimultaneity(Observer& observer);

	void loadOnGPU(GPUProgram& gpuProgram);

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

	void setWorldLine(std::map<int, WorldLine*>& worldLines) {
		worldLine = worldLines.at(worldLineID);
	}
};

