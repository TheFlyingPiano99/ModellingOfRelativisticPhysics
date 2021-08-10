#pragma once

#include "framework.h"
#include "Intersectable.h"

/*
* Represents a section of space-time, bordered by a 4D cone of light traveling towards and hiting the observer.
*/
class LightCone : public Intersectable {
	vec4 observer;

public:

	LightCone(vec4 _observer): observer(_observer) {
	}

	vec4 getObserver() {
		return observer;
	}

	const vec4 getObserver() const {
		return observer;
	}


};