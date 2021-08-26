#pragma once

#include "framework.h"

#include "Entity.h"

class Event : public Entity
{
	vec4 location;
	
public:
	Event(vec4 _location, std::string _name, std::string _desc = ""): Entity(_name, _desc), location(_location) {
	}

	vec4 getLocation() {
		return location;
	}

	const vec4 getLocation() const {
		return location;
	}

	void hover();
};







