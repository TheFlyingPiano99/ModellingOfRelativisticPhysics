#pragma once

#include "Intersectable.h"
#include "Geometry.h"

/*
* Hyper plane in space-time continuum described by a space-time location and a normal four-vector.
* These vectors are described in the absolute observer's coordinate system.
*/
class Hyperplane : public Intersectable {
	vec4 location;
	vec4 normal;

public:
	Hyperplane(vec4 _location, vec4 _normal): 
		Intersectable(RelTypes::IntersectionMode::hyperplane), 
		location(_location), 
		normal(_normal) {
	}
	
	static Hyperplane* simultaneousHyperplane(vec4 location, vec4 FVelocity) {
		return new Hyperplane(location, 
			normalize(vec4(-FVelocity.x, -FVelocity.y, -FVelocity.z, FVelocity.w)));
	}

	vec4 getLocation() {
		return location;
	}

	const vec4 getLocation() const {
		return location;
	}

	vec4 getNormal() {
		return normal;
	}

	const vec4 getNormal() const {
		return normal;
	}

	void setLocation(vec4 l) {
		location = l;
	}

	void setNormal(vec4 n) {
		normal = n;
	}

};

