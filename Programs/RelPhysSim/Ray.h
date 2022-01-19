#pragma once

#include "Geometry.h"

/*
* Represents a ray, what's used to select clicked object.
*/
struct Ray {
	vec3 pos, dir;

	Ray(vec3 _pos, vec3 _dir): pos(_pos), dir(_dir) {
	}
};