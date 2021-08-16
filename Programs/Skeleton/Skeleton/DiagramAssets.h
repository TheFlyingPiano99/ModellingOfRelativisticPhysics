#pragma once
#include "Geometry.h"
#include "Material.h"

/*
* Static getters for frequently used geomtries etc.
*/
class DiagramAssets {
	static SphereSurface* observerNodeGeometry;
	static ConeSurface* lightConeGeometry;
	static Material* observerMaterial;



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

	static Material* getObserverMaterial() {
		if (observerMaterial == nullptr) {
			observerMaterial = new Material(vec3(3, 1.5, 1), vec3(1, 0, 0.5), vec3(0.2, 0.2, 0.2), 3, 0.4f);
		}
		return observerMaterial;
	}

	/*
	* Must be called befor the application is closed!
	*/
	static void cleanUp() {
		delete observerNodeGeometry;
		delete lightConeGeometry;
		delete observerMaterial;
	}
};

