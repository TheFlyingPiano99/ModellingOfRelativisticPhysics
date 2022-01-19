#include "Camera.h"

#include <corecrt_math_defines.h>

void Camera::initBasic(const vec3 _eye, const vec3 _lookat, const vec3 _prefUp, const float _fov, const float _asp, const float _fp, const float _bp) {
	eye = _eye;
	lookat = _lookat;
	prefUp = normalize(_prefUp);
	fov = _fov;
	asp = _asp;
	fp = _fp;
	bp = _bp;
	setLookat(lookat);
}

void Camera::syncToObserver(const RelTypes::ObserverProperties& observerProperties) {
	locationFV = observerProperties.location;
	velocityFV = observerProperties.velocity;
	startPosFV = observerProperties.locationAtZero;
	vec3 updatedEye = vec3(locationFV.x, locationFV.y, locationFV.z);
	vec3 delta = updatedEye - eye;
	eye = updatedEye;
	lookat = lookat + delta;
	vec3 w = eye - lookat;
	vRight = normalize(cross(prefUp, w));
	vUp = normalize(cross(w, vRight));
}

void Camera::setLookat(const vec3 lat) {
	lookat = lat;
	vec3 w = eye - lookat;
	vRight = normalize(cross(prefUp, w));
	vUp = normalize(cross(w, vRight));
}

mat4 Camera::V() {
	vec3 w = normalize(eye - lookat);

	return mat4(vRight.x, vUp.x, w.x, 0,
		vRight.y, vUp.y, w.y, 0,
		vRight.z, vUp.z, w.z, 0,
		0, 0, 0, 1);
}

mat4 Camera::P() {
	float sy = 1.0f / tanf(fov / 2.0f);
	float a = -(fp + bp) / (bp - fp);
	float b = -2.0f * fp * bp / (bp - fp);
	return mat4(sy / asp, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, a, -1,
		0, 0, b, 0);
}

vec3 Camera::calculateRayStart(vec2 cPos)
{
	float scale = tanf(fov / 2.0f) * length(lookat - eye);
	return
		lookat
		+ vRight * asp * scale * cPos.x
		+ vUp * scale * cPos.y;
}

void Camera::loadOnGPU(GPUProgram& gpuProgram) {
	gpuProgram.setUniform(eye, "wEye");
}

void Camera::rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

	vec3 centered = lookat - eye;
	vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
	vec3 rotated3 = vec3(rotated.x, rotated.y, rotated.z);
	float l = length(rotated3);
	if (dot(rotated3, prefUp) / l > 0.999f || dot(rotated3, prefUp) / l < -0.999f) {
		return;
	}
	setLookat(rotated3 + eye);
}

void Camera::rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

	vec3 centered = eye - lookat;
	vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
	vec3 rotated3 = vec3(rotated.x, rotated.y, rotated.z);
	float l = length(rotated3);
		if (dot(rotated3, prefUp) / l > 0.999f || dot(rotated3, prefUp) / l < -0.999f) {
		return;
	}
	eye = rotated3 + lookat;
	setLookat(lookat);
}

void Camera::rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

	vec3 centeredEye = eye - point;
	vec3 centeredLookat = lookat - point;
	vec4 rotatedEye = vec4(centeredEye.x, centeredEye.y, centeredEye.z, 1) * hRotationM * vRotationM;
	vec4 rotatedLookat = vec4(centeredLookat.x, centeredLookat.y, centeredLookat.z, 1) * hRotationM * vRotationM;
	vec4 w4 = vec4(rotatedLookat - rotatedEye);
	vec3 w = normalize(vec3(w4.x, w4.y, w4.z));
	if (dot(w, prefUp) > 0.999f || dot(w, prefUp) < -0.999f) {
		return;
	}
	eye = vec3(rotatedEye.x, rotatedEye.y, rotatedEye.z) + point;
	lookat = vec3(rotatedLookat.x, rotatedLookat.y, rotatedLookat.z) + point;
	setLookat(lookat);
}

void Camera::move(vec3 delta)
{
	vec3 face = cross(prefUp, vRight);
	vec4 wDir4 = vec4(delta.x, delta.y, delta.z, 0)		// move in deltaZ = 0 plane.
		* mat4(
		face.x, face.y, face.z, 0,
		vRight.x, vRight.y, vRight.z, 0,
		prefUp.x, prefUp.y, prefUp.z, 0,
		0, 0, 0, 1);

	vec3 wDir = vec3(wDir4.x, wDir4.y, wDir4.z);
	eye = eye + wDir;
	lookat = lookat + wDir;
	startPosFV = startPosFV + vec4(wDir.x, wDir.y, wDir.z, 0);
	locationFV = locationFV + vec4(wDir.x, wDir.y, wDir.z, 0);
	// Not correct transformation of four vectors!!! Shoud be transformed in simultaneity!
}

void Camera::zoom(float delta) {
	fov *= delta;
	if (fov > M_PI)
		fov = M_PI;
	else if (fov < M_PI / 4.0f)
		fov = M_PI / 4.0f;
}

Ray Camera::getRayFromCameraCoord(vec2 cPos) {
	
	vec3 rayStart = calculateRayStart(cPos);

	return Ray(eye, normalize(rayStart - eye));
}
