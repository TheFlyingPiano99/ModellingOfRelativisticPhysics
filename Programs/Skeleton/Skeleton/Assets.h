#pragma once
#include "Geometry.h"
#include "Material.h"

/*
* Static getters for frequently used geomtries etc.
*/
class Assets {
	static SphereSurface* observerNodeGeometry;
	static ConeSurface* lightConeGeometry;
	static Material* observerMaterial;
	static Material* selectedObjectMaterial;
	static Material* selectedWorldLineMaterial;
	static OBJGeometry* cubeGeometry;


public:

	static Geometry* getObserverNodeGeometry() {
		if (observerNodeGeometry == nullptr) {
			observerNodeGeometry = new SphereSurface(0.5f);
			observerNodeGeometry->GenSurface(10, 10);
		}
		return observerNodeGeometry;
	}

	static Geometry* getLightConeGeomtry() {
		if (lightConeGeometry == nullptr) {
			lightConeGeometry = new ConeSurface();
			lightConeGeometry->GenSurface(50, 50);
		}
		return lightConeGeometry;
	}

	static Geometry* getCubeGeometry() {
		if (cubeGeometry == nullptr) {
			cubeGeometry = new OBJGeometry();
			cubeGeometry->load("../../../Resources/geometry/cube.obj");
		}
		return cubeGeometry;
	}

	static Material* getObserverMaterial() {
		if (observerMaterial == nullptr) {
			observerMaterial = new Material(vec3(3, 1.5, 1), vec3(1, 0, 0.5), vec3(0.2, 0.2, 0.2), 3, 0.4f);
		}
		return observerMaterial;
	}

	static Material* getSelectedObjectMaterial() {
		if (selectedObjectMaterial == nullptr) {
			selectedObjectMaterial = new Material(vec3(3, 3, 3), vec3(8, 9, 8), vec3(2, 2, 2), 3, 1.0f);
		}
		return selectedObjectMaterial;
	}

	static Material* getSelectedWorldLineMaterial() {
		if (selectedWorldLineMaterial == nullptr) {
			selectedWorldLineMaterial = new Material(vec3(3, 1.5, 1), vec3(1, 1, 1), vec3(0.2, 0.2, 0.2), 3, 1.0f);
			selectedWorldLineMaterial->setGlow(true);
		}
		return selectedWorldLineMaterial;
	}

	/*
	* Must be called befor the application is closed!
	*/
	static void cleanUp() {
		delete observerNodeGeometry;
		delete lightConeGeometry;
		delete observerMaterial;
		delete selectedObjectMaterial;
		delete selectedWorldLineMaterial;
		delete cubeGeometry;
	}
};

