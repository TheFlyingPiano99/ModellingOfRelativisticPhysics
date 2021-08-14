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
public:

	Observer(WorldLine* _worldLine, Material* _diagramMaterial, std::string _name = "", std::string _desc = "")
		: Entity(_name, _desc), worldLine(_worldLine), diagramMaterial(_diagramMaterial) {
	}

	~Observer() {
		delete worldLine;
	}

	vec4 getLocationAtAbsoluteTime(float t);

	vec4 getVelocityAtAbsoluteTime(float t);

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);

};

