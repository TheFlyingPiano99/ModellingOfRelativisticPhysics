#pragma once

#ifndef RELPHYSICS_H
#define RELPHYSICS_H

#include "framework.h"
#include "Exceptions.h"

namespace RelPhysics
{
	static const float speedOfLight = 1; // [m/m]
	static const vec4 absObserversFVelocity = vec4(0, 0, 0, 1);
	static const vec3 absObservers3DVelocity = vec3(0, 0, 0);

	/*
	* Converts 3D velocity [m/m] to four-velocity [four-velocity magnitude = c];
	*/
	static inline vec4 ToFourVelocity(const vec3 v) {
		return normalize(vec4(v.x, v.y, v.z, 1)) * speedOfLight;
	}

	/*
	* Converts four-velocity [magnitude = c] to 3D velocity [m/m];
	*/
	static inline vec3 To3DVelocity(const vec4 fv) {
		vec4 scaled = fv / fv.w;
		return vec3(scaled.x, scaled.y, scaled.z);
	}

	static float relativeVelocity(const vec3 v1, const vec3 v2) {
		return length(v1 - v2);
	}

	static float relativeVelocity (vec4 fv1, vec4 fv2) {
		return relativeVelocity(To3DVelocity(fv1), To3DVelocity(fv2));
	}

	/*
	* Takes relative velocity as argument.
	* Returns the Lorentz factor.
	*/
	static float lorentzFactor(float relVelocity) {
		if (relVelocity > speedOfLight) {
			throw new LightspeedExceededException;
		}
		return 1.0f / (1.0f - (relVelocity * relVelocity) / (speedOfLight * speedOfLight));
	}



};

#endif /*!RELPHYSICS_H*/
