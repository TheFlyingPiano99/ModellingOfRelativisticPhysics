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
	vec3 eye, lookat, vUp, vRight, prefUp;
	float fov, asp, nearPlane, farPlane;
	float orthographicScale = 20;
	//4-vectors:
	vec4 locationFV, velocityFV, startPosFV;
	bool usePerspective = true;
	RelTypes::DirectionMode directionMode = RelTypes::DirectionMode::free;

	void updateDirections(vec3 eye);

	void restoreNormalCamera();


public:

	/*
	* Must be called after construction, to set the necessary variables.
	*/
	void initBasic(const vec3 eye, const vec3 lookat, const vec3 prefUp, const float fov, const float asp, const float nearPlane, const float farPlane);

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
	mat4 getTranslationMatrix() {
		return TranslateMatrix(-eye);
	}

	/*
	* Return view matrix (Because of practical reasons: without the translate transformation!).
	* In diagram view mode the translate matrix must be multiplied with the MVP matrix: M * Translate * V * P
	*/
	mat4 getViewMatrix();
	
	mat4 getPerspectiveProjectionMatrix();

	mat4 getOrthographicProjectionMatrix();
	
	/*
	* Returns the currently selected projection matrix.
	* This can be perspective or orthographic.
	*/
	mat4 getActiveProjectionMatrix();
	/*
	* Transforms a camera space vector to world space so, that the world space position represented by the vector is in the "lookat plane".
	*/
	vec3 calculateRayStart(vec2 cPos);

	/*
	* Load camera on GPU.
	*/
	void loadOnGPU(GPUProgram& gpuProgram);

	/*
	* Return spacetime location of the camera / observer at the given moment.
	*/
	vec4 getLocationFV() {
		return locationFV;
	}

	/*
	* Return spacetime four-velocity of the camera / observer at the given moment.
	*/
	vec4 getVelocityFV() {
		return velocityFV;
	}

	vec4 getStartPosVF() {
		return startPosFV;
	}

	vec3 getEye() {
		return eye;
	}

	float getAspectRatio() {
		return asp;
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

	void setLookDirection(vec3 dir);

	/*
	* X...forward
	* Y...right
	* Z...up
	*/
	void move(vec3 delta);

	void zoom(float delta);

	Ray getRayFromCameraCoord(vec2 cPos);

	void selectDirectionMode(RelTypes::DirectionMode mode);

	void translateTo(vec3 pos);

	bool isPerspective() {
		return usePerspective;
	}

	float getOrthographicScale() {
		return orthographicScale;
	}
};
