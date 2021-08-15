#pragma once
#include "Geometry.h"

/*
* Static getters for frequently used geomtries etc.
*/
class DiagramAssets {
	static SphereSurface* observerNodeGeometry;
	static ConeSurface* lightConeGeometry;

public:

	static Geometry* getObserverNodeGeometry() {
		if (observerNodeGeometry == nullptr) {
			observerNodeGeometry = new SphereSurface(1);
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

	/*
	* Must be called befor the application is closed!
	*/
	static void cleanUp() {
		delete observerNodeGeometry;
		delete lightConeGeometry;
	}
};

