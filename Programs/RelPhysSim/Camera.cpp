#include "Camera.h"

#include <corecrt_math_defines.h>

void Camera::updateDirections(vec3 updatedEye)
{
	vec3 delta = updatedEye - eye;
	eye = updatedEye;
	lookat = lookat + delta;
	vec3 w = eye - lookat;
	float d = dot(normalize(w), vec3(0, 0, 1));
	if (abs(d) > 0.99f) {
		if (d > 0) {
			preferedUp = vec3(0, 1, 0);
		}
		else {
			preferedUp = vec3(0, -1, 0);
		}
	}
	else {
		preferedUp = vec3(0, 0, 1);
	}

	right = normalize(cross(preferedUp, w));
	up = normalize(cross(w, right));
}

void Camera::restoreNormalCamera() {
	usePerspective = true;
	directionMode = RelTypes::DirectionMode::free;
	updateDirections(eye);
}

void Camera::initBasic(const vec3 _eye, 
	const vec3 _lookat,
	const vec3 _prefUp,
	const float _fov, 
	const float _asp,
	const float _fp,
	const float _bp) {
	eye = _eye;
	lookat = _lookat;
	preferedUp = normalize(_prefUp);
	fov = _fov;
	asp = _asp;
	nearPlane = _fp;
	farPlane = _bp;
	setLookat(lookat);
}

void Camera::syncToObserver(const RelTypes::ObserverProperties& observerProperties) {
	vec3 updatedEye = 
		vec3(observerProperties.location.x, 
		observerProperties.location.y, 
		observerProperties.location.z);
	updateDirections(updatedEye);
}

void Camera::setLookat(const vec3 lat) {
	lookat = lat;
	vec3 w = eye - lookat;
	right = normalize(cross(preferedUp, w));
	up = normalize(cross(w, right));
}

/*
* Return translate matrix.
*/

const mat4 Camera::getTranslationMatrix() const {
	return TranslateMatrix(-eye);
}

const mat4 Camera::getViewMatrix() const {
	vec3 w = normalize(eye - lookat);

	return mat4(right.x, up.x, w.x, 0,
		right.y, up.y, w.y, 0,
		right.z, up.z, w.z, 0,
		0, 0, 0, 1);
}



const mat4 Camera::getPerspectiveProjectionMatrix() const {
	float sy = 1.0f / tanf(fov / 2.0f);
	float a = -(nearPlane + farPlane) / (farPlane - nearPlane);
	float b = -2.0f * nearPlane * farPlane / (farPlane - nearPlane);
	return mat4(sy / asp, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, a, -1,
		0, 0, b, 0);
}

const mat4 Camera::getOrthographicProjectionMatrix() const {
	float sy = 1.0f / tanf(fov / 2.0f) * orthographicScale;
	float r, l, t, b;
	t = sy;
	b = -sy;
	r = sy * asp;
	l = -sy * asp;
	mat4 getModellMatrix = mat4(
		2.0f / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2.0f / (t - b), 0, -(t + b) / (t - b),
		0, 0, -2.0f / (farPlane - nearPlane), -(farPlane + nearPlane) / (farPlane - nearPlane),
		0, 0, 0, 1
	);
	return transpose(getModellMatrix);
}

const mat4 Camera::getActiveProjectionMatrix() const {
	if (usePerspective) {
		return getPerspectiveProjectionMatrix();
	}
	return getOrthographicProjectionMatrix();
}

const vec3 Camera::calculateRayStart(vec2 cPos) const {
	float scale = tanf(fov / 2.0f) * length(lookat - eye);
	float sy = 1.0f / tanf(fov / 2.0f) * 20.0f;
	vec3 center = (usePerspective) ? lookat 
		: eye + normalize(lookat - eye) * nearPlane;
	return
		center
		+ right * asp * ((usePerspective) ? scale : sy) * cPos.x
		+ up * ((usePerspective) ? scale : sy) * cPos.y;
}

void Camera::loadOnGPU(GPUProgram& gpuProgram) const {
	gpuProgram.setUniform(eye, "wEye");
}
vec3 Camera::getEye() const {
	return eye;
}
float Camera::getAspectRatio() const {
	return asp;
}
const vec3 Camera::getPrefUp() const {
	return preferedUp;
}
const vec3 Camera::getLookDir() const {
	return normalize(lookat - eye);
}
const vec3 Camera::getRight() const {
	return right;
}
const vec3 Camera::getUp() const {
	return up;
}
void Camera::rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, preferedUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, right);

	vec3 centered = lookat - eye;
	vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
	vec3 rotated3 = vec3(rotated.x, rotated.y, rotated.z);
	float l = length(rotated3);
	if (dot(rotated3, preferedUp) / l > 0.999f || dot(rotated3, preferedUp) / l < -0.999f) {
		return;
	}
	setLookat(rotated3 + eye);
	restoreNormalCamera();
}

void Camera::rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, preferedUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, right);

	vec3 centered = eye - lookat;
	vec4 rotated = vec4(centered.x, centered.y, centered.z, 1) * hRotationM * vRotationM;
	vec3 rotated3 = vec3(rotated.x, rotated.y, rotated.z);
	float l = length(rotated3);
		if (dot(rotated3, preferedUp) / l > 0.999f || dot(rotated3, preferedUp) / l < -0.999f) {
		return;
	}
	eye = rotated3 + lookat;
	setLookat(lookat);
	restoreNormalCamera();
}

void Camera::rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point) {
	mat4 vRotationM = RotationMatrix(verticalAxisAngle / fov, preferedUp);	// Scaled by fov, to avoid fast movement, while zoomed.
	mat4 hRotationM = RotationMatrix(horizontalAxisAngle / fov, right);

	vec3 centeredEye = eye - point;
	vec3 centeredLookat = lookat - point;
	vec4 rotatedEye = vec4(centeredEye.x, centeredEye.y, centeredEye.z, 1) * hRotationM * vRotationM;
	vec4 rotatedLookat = vec4(centeredLookat.x, centeredLookat.y, centeredLookat.z, 1) * hRotationM * vRotationM;
	vec4 w4 = vec4(rotatedLookat - rotatedEye);
	vec3 w = normalize(vec3(w4.x, w4.y, w4.z));
	if (dot(w, preferedUp) > 0.999f || dot(w, preferedUp) < -0.999f) {
		return;
	}
	eye = vec3(rotatedEye.x, rotatedEye.y, rotatedEye.z) + point;
	lookat = vec3(rotatedLookat.x, rotatedLookat.y, rotatedLookat.z) + point;
	setLookat(lookat);
	restoreNormalCamera();
}

void Camera::setLookDirection(vec3 dir)
{
	lookat = eye + normalize(dir) * length(lookat - eye);
}

void Camera::move(vec3 delta)
{
	vec3 face = cross(preferedUp, right);
		* mat4(
		face.x, face.y, face.z, 0,
		right.x, right.y, right.z, 0,
		preferedUp.x, preferedUp.y, preferedUp.z, 0,
		0, 0, 0, 1);

	eye = eye + delta;
	lookat = lookat + delta;
}

void Camera::zoom(float delta) {
	fov *= delta;
	if (fov > M_PI)
		fov = M_PI;
	else if (fov < M_PI / 4.0f)
		fov = M_PI / 4.0f;
}

const Ray Camera::getRayFromCameraCoord(vec2 cPos) {
	
	vec3 rayStart = calculateRayStart(cPos);

	return Ray(rayStart, (usePerspective)? normalize(rayStart - eye) : normalize(lookat - eye));
}

void Camera::selectDirectionMode(RelTypes::DirectionMode mode)
{
	directionMode = mode;
	usePerspective = (mode == RelTypes::DirectionMode::free);
	preferedUp = vec3(0, 0, 1);
	switch (directionMode)
	{
	case RelTypes::DirectionMode::Xlocked:
			setLookDirection(vec3(1, 0, 0));
			break;
	case RelTypes::DirectionMode::Ylocked:
			setLookDirection(vec3(0, 1, 0));
			break;
	case RelTypes::DirectionMode::Zlocked:
			setLookDirection(vec3(0, 0, 1));
			break;
	case RelTypes::DirectionMode::minusXlocked:
			setLookDirection(vec3(-1, 0, 0));
			break;
	case RelTypes::DirectionMode::minusYlocked:
			setLookDirection(vec3(0, -1, 0));
			break;
	case RelTypes::DirectionMode::minusZlocked:
			setLookDirection(vec3(0, 0, -1));
			break;
	case RelTypes::DirectionMode::free:
		break;
	default:
		break;
	}
	updateDirections(eye);
}

void Camera::translateTo(vec3 pos)
{
	vec3 w = lookat - eye;
	eye = pos;
	lookat = eye + w;
}

const bool Camera::isPerspective() const {
	return usePerspective;
}

const float Camera::getOrthographicScale() const {
	return orthographicScale;
}
