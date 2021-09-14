#pragma once
#include "framework.h"
#include "Ray.h"

struct Plane
{
	vec3 pos, norm;

	Plane(vec3 _pos, vec3 _norm): pos(_pos), norm(_norm) {

	}
};

inline static vec3 intersect(const Plane& plane, const Ray& ray) {
	float t = dot(plane.pos - ray.pos, plane.norm) / dot(ray.dir, plane.norm);
	return ray.pos + ray.dir * t;
}

