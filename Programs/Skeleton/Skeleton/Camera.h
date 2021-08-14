#pragma once
#include "framework.h"
#include "Hyperplane.h"

/*
* Viewing camera.
*/
class Camera {
	vec3 eye, lookat, vUp, vRight, prefUp;
	float fov, asp, fp, bp;
	//4-vectors:
	vec4 locationFV, velocityFV, startPosFV;


public:

	Hyperplane getHyperplane();

	void initBasic(const vec3 eye, const vec3 lookat, const vec3 prefUp, const float fov, const float asp, const float fp, const float bp);

	void syncToObserver(const vec4 _location, const vec4 _velocity, const vec4 _startPos);

	void setLookat(const vec3 lat);

	mat4 Translate() {
		return TranslateMatrix(-eye);
	}

	mat4 V() {
		vec3 w = normalize(eye - lookat);

		return mat4(vRight.x, vUp.x, w.x, 0,
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

	mat4 invV() {
		//Todo
		return mat4();
	}

	mat4 invP() {
		//Todo
		return mat4();
	}

	void loadOnGPU(GPUProgram& gpuProgram);

	vec4 getLocationFV() {
		return locationFV;
	}

	vec4 getVelocityFV() {
		return velocityFV;
	}

	vec3 getEye() {
		return eye;
	}

	void rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle);

	void rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle);

	void rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point);

	void zoom(float delta);

};
