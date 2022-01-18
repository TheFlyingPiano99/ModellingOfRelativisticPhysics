#pragma once

#include "framework.h"
#include "Intersectable.h"

/*
* Represents a section of space-time, bordered by a 4D cone of light traveling towards and hiting the observer.
*/
class LightCone : public Intersectable {
	vec4 location;

public:

	LightCone(vec4 _location): Intersectable(RelTypes::IntersectionMode::lightCone), location(_location) {
	}

	vec4 getLocation() {
		return location;
	}

	const vec4 getLocation() const {
		return location;
	}


};