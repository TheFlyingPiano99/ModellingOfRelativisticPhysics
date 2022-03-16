#pragma once

#include "Object.h"
#include "WorldLine.h"

#define NAME_BUFFER_SIZE 75

class Initialiser
{
	RelTypes::ObjectType objectType = RelTypes::ObjectType::none;
	WorldLine::Type worldLineType = WorldLine::Type::GeodeticLine;


public:
	vec3 positionAtZeroT = vec3(0.0f);
	vec3 velocityAtZeroT = vec3(0.0f);
	vec3 centerOfRotation = vec3(0.0f);
	char name[NAME_BUFFER_SIZE] = "Object";

	Initialiser() {}

	bool finish(Object*& object, WorldLine*& worldLine);

	RelTypes::ObjectType getObjectType();

	void setObjectType(RelTypes::ObjectType type) {
		this->objectType = type;
	}

	WorldLine::Type getWorldLineType() {
		return worldLineType;
	}

	void setWorldLineType(WorldLine::Type type) {
		worldLineType = type;
	}

};

