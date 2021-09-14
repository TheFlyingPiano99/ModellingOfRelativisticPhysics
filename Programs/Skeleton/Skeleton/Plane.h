#pragma once
#include "framework.h"

struct Plane
{
	vec3 pos, norm;

	Plane(vec3 _pos, vec3 _norm): pos(_pos), norm(_norm) {

	}
};

