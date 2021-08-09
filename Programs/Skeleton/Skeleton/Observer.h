#pragma once

#include "framework.h"

#include "Camera.h"
#include "Entity.h"
#include "WorldLine.h"


class Observer : public Entity
{
	WorldLine* worldLine;

public:

	Observer(WorldLine* _worldLine, std::string _name, std::string _desc = "")
		: Entity(_name, _desc), worldLine(_worldLine) {
	}

	~Observer() {
	}

	Camera* getCameraAtAbsoluteTime(float t) {
		vec4 location = worldLine->getLocationAtAbsoluteTime(t);
		vec4 fVelocity = worldLine->getVelocityAtAbsoluteTime(t);

		//Todo
		return new Camera();
	}

	Camera* getCameraAtProperTime(float tau) {
		vec4 location = worldLine->getLocationAtProperTime(tau);
		vec4 fVelocity = worldLine->getVelocityAtProperTime(tau);

		//Todo
		return new Camera();
	}
};

