#pragma once
#include "framework.h"

/*
* Viewing camera.
*/
class Camera {
	vec3 eye, lookat, vUp, vRight, prefUp;
	float fov, asp, fp, bp;
	//4-vectors:
	vec4 locationFV, velocityFV;


public:
	void initBasic(const vec3 lookat, const vec3 prefUp, const float fov, const float asp, const float fp, const float bp) {
		this->lookat = lookat;
		this->prefUp = prefUp;
		this->fov = fov;
		this->asp = asp;
		this->fp = fp;
		this->bp = bp;
	}

	void update(const vec4 _location, const vec4 _velocity) {
		locationFV = _location;
		velocityFV = _velocity;
		vec3 updatedEye = vec3(locationFV.x, locationFV.y, locationFV.z);
		vec3 delta = updatedEye - eye;
		eye = updatedEye;
		lookat = lookat + delta;
		vec3 w = eye - lookat;
		vRight = normalize(cross(prefUp, w));
		vUp = normalize(cross(w, vRight));
	}

	void setLookat(const vec3 lat) {
		lookat = lat;
	}

	mat4 V() {
		vec3 w = normalize(eye - lookat);
		return TranslateMatrix(-eye) * mat4(vRight.x, vUp.x, w.x, 0,
			vRight.y, vUp.y, w.y, 0,
			vRight.z, vUp.z, w.z, 0,
			0, 0, 0, 1);
	}

	mat4 P() {
		float sy = tanf(fov / 2.0f);
		float a = -(fp + bp) / (bp - fp);
		float b = -2.0f * bp / (bp - fp);
		return mat4(sy * asp, 0, 0, 0,
			0, sy, 0, 0,
			0, 0, a, -1,
			0, 0, b, 0);
	}

	void Animate(float dt) {
		eye = eye + vec3(0, 0, 1) * dt;
	}

	void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(eye, "wEye");
	}
};
