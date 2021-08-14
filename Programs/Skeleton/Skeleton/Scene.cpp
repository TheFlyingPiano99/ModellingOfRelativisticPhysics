#include "Scene.h"
#include "Geometry.h"
#include "Material.h"

void Scene::Initialise()
{
	//View:
	view = new RealTime3DView(this);

	//Camera:
	realTime3DCamera = new Camera();
	realTime3DCamera->initBasic(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowHeight / (float)windowWidth, 0.02f, 3.0f);
	diagramCamera = new Camera();
	diagramCamera->initBasic(20 * vec3(1,1,1), 19 * vec3(1, 1, 1), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowHeight / (float)windowWidth, 0.02f, 3.0f);

	if (viewMode == realTime3D) {
		activeCamera = realTime3DCamera;
	}
	else {
		activeCamera = diagramCamera;
	}

	//LightSources:----------------------------------------------
	lights.push_back(new LightSource(vec3(0, 0, 0), vec3(0.1, 0.1, 0.1), 0));
	lights.push_back(new LightSource(vec3(-80, 0, 0), vec3(1000, 1000, 1000), 0));
	diagramLights.push_back(new LightSource(vec3(-10, -10, -10), vec3(10000, 10000, 10000), 0));
	diagramLights.push_back(new LightSource(vec3(0, 0, 0), vec3(10000, 10000, 10000), 0));

	//Observers:-------------------------------------------------
	//1.:
	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	observers.push_back(observer);

	//2.:
	wrdln = new GeodeticLine(vec3(0.0f, -6.0f, 0.0f), vec3(0.0f, 0.93f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs2", "An observer");
	observers.push_back(observer);


	//3.:
	wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.2f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs3", "An observer");
	observers.push_back(observer);

	//Objects:----------------------------------------------------
	wrdln = new GeodeticLine(vec3(3.0f, -6.0f, 0.0f), vec3(0.0f, 0.93f, 0.0f), "Obj1's world line");
	objects.push_back(Object::createEarth(wrdln));

	for (int i = 0; i < 10; i++) {
		wrdln = new GeodeticLine(vec3(3.0f, -6.0f + i * 3, -1.0f), vec3(0.0f, 0.0f, 0.0f), "Obj1's world line");
		objects.push_back(Object::createEarth(wrdln));
	}

	//Background:------------------------------------------------------
	background = new Background();
	
	//Other:
	toggleCurrentObserver();

}

void Scene::Draw(GPUProgram& gpuProgram) {
	if (currentObserver != nullptr || viewMode == diagram) {
		//Prefase:
		gpuProgram.setUniform(RelPhysics::speedOfLight, "speedOfLight");
		gpuProgram.setUniform(intersectionMode, "intersectionMode");
		gpuProgram.setUniform(dopplerMode, "dopplerMode");
		gpuProgram.setUniform(doLorentz, "doLorentz");
		gpuProgram.setUniform(viewMode, "viewMode");
		gpuProgram.setUniform(vec3(0.05, 0.05, 0.05), "La");
		activeCamera->loadOnGPU(gpuProgram);

		view->Draw(gpuProgram);
	}
}

void Scene::toggleCurrentObserver() {
	static int currentIdx = -1;
	if (!observers.empty()) {	// Incrementation with overflow
		currentIdx = (observers.size() > currentIdx + 1) ? currentIdx + 1 : 0;
		currentObserver = observers.at(currentIdx);
		realTime3DCamera->syncToObserver(currentObserver->getLocationAtAbsoluteTime(absoluteTimeSpent),
			currentObserver->getVelocityAtAbsoluteTime(absoluteTimeSpent),
			currentObserver->getLocationAtAbsoluteTime(0.0f));
	}
}

// Camera controls:

void Scene::moveCamera(float cx, float cy) {
	static float camSpeed = 0.01f;
	if (viewMode == realTime3D) {
		realTime3DCamera->rotateAroundEye(cx, -cy);
	}
	else if (viewMode == diagram) {
		diagramCamera->rotateAroundPoint(cx, -cy, vec3(0, 0, 0));
	}
}

void Scene::toggleViewMode() {
	viewMode = (ViewMode)((2 > viewMode + 1) ? (viewMode + 1) : 0);
	switch (viewMode)
	{
	case ViewMode::realTime3D:
		activeCamera = realTime3DCamera;
		delete view;
		view = new RealTime3DView(this);
		break;
	case ViewMode::diagram:
		activeCamera = diagramCamera;
		delete view;
		view = new DiagramView(this);
		break;
	default:
		break;
	}
}
