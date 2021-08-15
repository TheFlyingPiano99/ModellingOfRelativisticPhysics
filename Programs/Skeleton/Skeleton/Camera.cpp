#include "Camera.h"

Hyperplane Camera::getHyperplane() {
	return Hyperplane::simultaneousHyperplane(
		locationFV,
		velocityFV
	);
	//return Hyperplane(locationFV, velocityFV);
}

void Camera::initBasic(const vec3 _eye, const vec3 _lookat, const vec3 _prefUp, const float _fov, const float _asp, const float _fp, const float _bp) {
	eye = _eye;
	lookat = _lookat;
	prefUp = _prefUp;
	fov = _fov;
	asp = _asp;
	fp = _fp;
	bp = _bp;
	setLookat(lookat);
}

void Camera::syncToObserver(const vec4 _location, const vec4 _velocity, const vec4 _startPos) {
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

void Camera::setLookat(const vec3 lat) {
	lookat = lat;
	vec3 w = eye - lookat;
	vRight = normalize(cross(prefUp, w));
	vUp = normalize(cross(w, vRight));
}

void Camera::loadOnGPU(GPUProgram& gpuProgram) {
	gpuProgram.setUniform(eye, "wEye");
}

void Camera::rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

	vec3 centered = lookat - eye;
	vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
	setLookat(vec3(rotated.x, rotated.y, rotated.z) + eye);
}

void Camera::rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, prefUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, vRight);

	vec3 centered = eye - lookat;
	vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
	eye = vec3(rotated.x, rotated.y, rotated.z) + lookat;
	setLookat(lookat);
}

void Camera::rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point) {
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

void Camera::zoom(float delta) {
	fov *= delta;
	if (fov > M_PI)
		fov = M_PI;
	else if (fov < M_PI / 4.0f)
		fov = M_PI / 4.0f;
}
