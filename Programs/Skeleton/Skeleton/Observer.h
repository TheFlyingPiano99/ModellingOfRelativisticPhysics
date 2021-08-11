#pragma once

#include "framework.h"

#include "Camera.h"
#include "Entity.h"
#include "WorldLine.h"


class Observer : public Entity
{
	WorldLine* worldLine = NULL;

public:

	Observer(WorldLine* _worldLine, std::string _name = "", std::string _desc = "")
		: Entity(_name, _desc), worldLine(_worldLine) {
	}

	~Observer() {
		delete worldLine;
	}

	vec4 getLocationAtAbsoluteTime(float t);

	vec4 getVelocityAtAbsoluteTime(float t);

};

