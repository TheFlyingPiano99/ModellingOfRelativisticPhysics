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

	Hyperplane getHyperplane() {
		return Hyperplane::simultaneousHyperplane(
			locationFV,
			velocityFV
		);
		//return Hyperplane(locationFV, velocityFV);
	}

	void initBasic(const vec3 eye, const vec3 lookat, const vec3 prefUp, const float fov, const float asp, const float fp, const float bp) {
		this->eye = eye;
		this->lookat = lookat;
		this->prefUp = prefUp;
		this->fov = fov;
		this->asp = asp;
		this->fp = fp;
		this->bp = bp;
		setLookat(lookat);
	}

	void syncToObserver(const vec4 _location, const vec4 _velocity, const vec4 _startPos) {
		locationFV = _location;
		velocityFV = _velocity;
		startPosFV = _startPos;
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
		vec3 w = eye - lookat;
		vRight = normalize(cross(prefUp, w));
		vUp = normalize(cross(w, vRight));
	}

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

	void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(velocityFV, "observersVelocity");
		gpuProgram.setUniform(locationFV, "observersLocation");
		gpuProgram.setUniform(startPosFV, "observersStartPos");
		gpuProgram.setUniform(eye, "wEye");
	}

	vec4 getLocationFV() {
		return locationFV;
	}

	vec4 getVelocityFV() {
		return velocityFV;
	}

	vec3 getEye() {
		return eye;
	}

	void rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle) {
		mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
		mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

		vec3 centered = lookat - eye;
		vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
		setLookat(vec3(rotated.x, rotated.y, rotated.z) + eye);
	}

	void rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle) {
		mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
		mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

		vec3 centered = eye - lookat;
		vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
		eye = vec3(rotated.x, rotated.y, rotated.z) + lookat;
		setLookat(lookat);
	}

	void rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point) {
		mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
		mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

		vec3 centeredEye = eye - point;
		vec3 centeredLookat = lookat - point;
		vec4 rotatedEye = vec4(centeredEye.x, centeredEye.y, centeredEye.z, 1) * hRotationM * vRotationM;
		vec4 rotatedLookat = vec4(centeredLookat.x, centeredLookat.y, centeredLookat.z, 1) * hRotationM * vRotationM;
		eye = vec3(rotatedEye.x, rotatedEye.y, rotatedEye.z) + point;
		lookat = vec3(rotatedLookat.x, rotatedLookat.y, rotatedLookat.z) + point;
		setLookat(lookat);
	}

	void zoom(float delta) {
		fov *= delta;
		if (fov > M_PI)
			fov = M_PI;
		else if (fov < M_PI / 4.0f)
			fov = M_PI / 4.0f;
	}

};
