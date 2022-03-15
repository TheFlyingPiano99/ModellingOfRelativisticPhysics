#pragma once
#include "Hyperplane.h"
#include "Ray.h"
#include "GPUProgram.h"
#include "Geometry.h"

namespace RelTypes {
	enum DirectionMode {
		free,
		Xlocked,
		Ylocked,
		Zlocked,
		minusXlocked,
		minusYlocked,
		minusZlocked
	};
}

/*
* Viewing camera.
*/
class Camera {
	vec3 eye, lookat, up, right, preferedUp;
	float fov, asp, nearPlane, farPlane;
	float orthographicScale = 20;
	float zoomFactor = 1.0f;

	bool usePerspective = true;
	RelTypes::DirectionMode directionMode = RelTypes::DirectionMode::free;

	void updateDirections(vec3 eye);

	void restoreNormalCamera();


public:

	/*
	* Must be called after construction, to set the necessary variables.
	*/
	void initBasic(const vec3 eye, const vec3 lookat, const vec3 preferedUp, const float fov, const float asp, const float nearPlane, const float farPlane);

	/*
	* Update camera to allignt with the observer.
	*/
	void syncToObserver(const RelTypes::ObserverProperties& observerProperties);

	/*
	* Creates an orthogonal system, used for the View, Projection transformations.
	*/
	void setLookat(const vec3 lat);

	/*
	* Return translate matrix.
	*/
	const mat4 getTranslationMatrix() const;

	/*
	* Return view matrix (Because of practical reasons: without the translate transformation!).
	* In diagram view mode the translate matrix must be multiplied with the MVP matrix: M * Translate * V * P
	*/
	const mat4 getViewMatrix() const;
	
	const mat4 getPerspectiveProjectionMatrix() const;

	const mat4 getOrthographicProjectionMatrix() const;
	
	/*
	* Returns the currently selected projection matrix.
	* This can be perspective or orthographic.
	*/
	const mat4 getActiveProjectionMatrix() const;
	/*
	* Transforms a camera space vector to world space so, that the world space position represented by the vector is in the "lookat plane".
	*/
	const vec3 calculateRayStart(vec2 cPos) const;

	/*
	* Load camera on GPU.
	*/
	void loadOnGPU(GPUProgram& gpuProgram) const;

	vec3 getEye() const;

	float getAspectRatio() const;

	const vec3 getPreferedUp() const;

	const vec3 getLookDir() const;

	const vec3 getRight() const;
	
	const vec3 getUp() const;

	void rotateAroundEye(float verticalAxisAngle, float horizontalAxisAngle);

	void rotateAroundLookat(float verticalAxisAngle, float horizontalAxisAngle);

	void rotateAroundPoint(float verticalAxisAngle, float horizontalAxisAngle, vec3 point);

	void setLookDirection(vec3 dir);

	/*
	* X...forward
	* Y...right
	* Z...up
	*/
	void move(vec3 delta);

	void zoom(float delta);

	const Ray getRayFromCameraCoord(vec2 cPos);

	void selectDirectionMode(RelTypes::DirectionMode mode);

	void translateTo(vec3 pos);

	const bool isPerspective() const;

	const float getOrthographicScale() const;

	const float getFOV() const;

	const float getZoomFacor() const;
};
