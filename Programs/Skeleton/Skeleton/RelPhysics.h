#pragma once

#ifndef RELPHYSICS_H
#define RELPHYSICS_H

#include "framework.h"
#include "Exceptions.h"

namespace RelPhysics
{
	static const float speedOfLight = 1; // [m/m]
	static const vec4 absObservers4Velocity = vec4(0, 0, 0, 1);
	static const vec3 absObservers3DVelocity = vec3(0, 0, 0);

	/*
	* Load constants on GPU.
	*/
	static inline void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(speedOfLight, "speedOfLight");
	}

	/*
	* Converts 3D velocity [m/m] to four-velocity [magnitude = c];
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

	/*
	* Calculates difference between the two velocity vectors.
	*/
	static float relativeVelocity(const vec3 v1, const vec3 v2) {
		return length(v1 - v2);
	}

	/*
	* Calculates difference between the two velocity 4-vectors.
	*/
	static float relativeVelocity (vec4 fv1, vec4 fv2) {
		return relativeVelocity(To3DVelocity(fv1), To3DVelocity(fv2));
	}

	/*
	* Returns velocity 4-vector of geodetic world line intersecting both events.
	*/
	static vec4 tangentFourVelocity(vec4 event1, vec4 event2) {
		vec4 dir = event2 - event1;
		return normalize(dir) * speedOfLight * ((dir.w > 0.0f)? 1 : -1);
	}

	/*
	* Takes relative velocity as argument.
	* Returns the Lorentz factor, used to tranform observed length and time between two frames.
	*/
	/*
	* Takes relative velocity as argument.
	* Returns the Lorentz factor.
	*/
	static float lorentzFactor(float relVelocity) {
		if (relVelocity > speedOfLight) {
			throw new LightspeedExceededException;
		}
		return 1.0f / sqrtf(1.0f - (relVelocity * relVelocity) / (speedOfLight * speedOfLight));
	}

	/*
	* Transforms an event's location in space-time to the frame, which is mooving with relVelocity relative to the original frame.
	*/
	static vec4 lorentzTransformation(vec4 toTransform, vec3 relVelocity) {
		vec3 v = relVelocity;
		float vLength = length(v);
		if (vLength > 0.0f && vLength < speedOfLight) {
			float gamma = lorentzFactor(vLength);

			vec3 r = vec3(toTransform.x, toTransform.y, toTransform.z);
			float t = toTransform.w;
			vec3 n = normalize(v);
			float tTrans = gamma * (t - dot(vLength * n, r) / speedOfLight / speedOfLight);
			vec3 rTrans = r + (gamma - 1) * dot(r, n) * n - gamma * t * vLength * n;
			return vec4(rTrans.x, rTrans.y, rTrans.z, tTrans);
		}
		else {
			return toTransform;
		}
	}

	static vec3 lorentzTransformationOfVelocity(vec3 toTransform, vec3 relVelocity) {
		float gammaV = 1 / sqrtf(1 - dot(relVelocity, relVelocity) / speedOfLight / speedOfLight);
		return (1 / (1 - dot(relVelocity, toTransform) / speedOfLight / speedOfLight))
			* (toTransform / gammaV - relVelocity
				+ (1 / speedOfLight / speedOfLight) * (gammaV / (gammaV + 1))
				* dot(toTransform, relVelocity) * relVelocity);
	}

	static vec4 galileanTransformation(vec4 toTransform, vec3 relVelocity) {
		vec3 r = vec3(toTransform.x, toTransform.y, toTransform.z);
		float t = toTransform.w;
		vec3 v = relVelocity;
		vec3 rTrans = r - v * t;
		float tTrans = t;
		return vec4(rTrans.x, rTrans.y, rTrans.z, tTrans);
	}

	static vec3 galileanTransformationOfVelocity(vec3 toTransform, vec3 relVelocity) {
		return toTransform - relVelocity;
	}
};

#endif /*!RELPHYSICS_H*/
