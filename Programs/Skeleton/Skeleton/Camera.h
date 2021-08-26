#pragma once
#include "framework.h"
#include "Hyperplane.h"
#include "Ray.h"

/*
* Viewing camera.
*/
class Camera {
	vec3 eye, lookat, vUp, vRight, prefUp;
	float fov, asp, fp, bp;
	//4-vectors:
	vec4 locationFV, velocityFV, startPosFV;


public:


	void initBasic(const vec3 eye, const vec3 lookat, const vec3 prefUp, const float fov, const float asp, const float fp, const float bp);

	void syncToObserver(const vec4 _location, const vec4 _velocity, const vec4 _startPos);

	void setLookat(const vec3 lat);

	mat4 Translate() {
		return TranslateMatrix(-eye);
	}

	mat4 V();

	mat4 P();


	void loadOnGPU(GPUProgram& gpuProgram);

	vec4 getLocationFV() {
		return locationFV;
	}

	vec4 getVelocityFV() {
		return velocityFV;
	}

	vec4 getStartPosVF() {
		return startPosFV;
	}

	vec3 getEye() {
		return eye;
	}

	vec3 getPrefUp() {
		return prefUp;
	}

	vec3 getLookDir() {
		return normalize(lookat - eye);
	}

	vec3 getRight() {
		return vRight;
	}
	vec3 getUp() {
		return vUp;
	}

	void rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle);

	void rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle);

	void rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point);

	void zoom(float delta);

	Ray getRayFromCameraCoord(vec2 cPos);

};
