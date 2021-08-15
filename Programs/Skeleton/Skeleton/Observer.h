#pragma once

#include "framework.h"

#include "Camera.h"
#include "Entity.h"
#include "WorldLine.h"
#include "Material.h"


class Observer : public Entity
{
	WorldLine* worldLine = NULL;
	Material* diagramMaterial = NULL;
	float currentProperTime = 0.0f;
public:

	Observer(WorldLine* _worldLine, Material* _diagramMaterial, std::string _name = "", std::string _desc = "")
		: Entity(_name, _desc), worldLine(_worldLine), diagramMaterial(_diagramMaterial) {
	}

	~Observer() {
		delete worldLine;
	}

	vec4 getLocation();

	vec4 getVelocity();

	vec4 getStartPos();

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);
	void DrawHyperplane(GPUProgram& gpuProgram, Camera& camera);
	void DrawLightCone(GPUProgram& gpuProgram, Camera& camera);
	void DrawNode(GPUProgram& gpuProgram, Camera& camera);
	void DrawExtras(GPUProgram& gpuProgram, Camera& camera);

	/*
	* Receives time in absolute frame.
	*/
	void setTimeAtAbsoluteTime(float t);

	/*
	* Receives delta time in proper frame.
	*/
	void increaseTimeByDelta(float deltaTau);

	void syncCamera(Camera* camera);

	void loadOnGPU(GPUProgram& gpuProgram);
};

