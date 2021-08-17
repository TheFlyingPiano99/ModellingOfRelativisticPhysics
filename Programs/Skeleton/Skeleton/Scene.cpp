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
	lights.push_back(new LightSource(vec3(70, 50, 50), vec3(1000, 1000, 1000), 0));
	lights.push_back(new LightSource(vec3(-60, -10, 10), vec3(500, 500, 500), 1));
	lights.push_back(new LightSource(vec3(-1, 1, 1), vec3(0.01, 0.01, 0.01), 2));
	diagramLights.push_back(new LightSource(vec3(10, 10, 10), vec3(0, 0, 0), 0));
	diagramLights.push_back(new LightSource(vec3(100, 100, 100), vec3(100000, 100000, 100000), 1));

	//Observers:-------------------------------------------------
	//1.:
	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	observers.push_back(observer);

	//2.:
	wrdln = new GeodeticLine(vec3(0.0f, -6.0f, 0.0f), vec3(0.0f, 0.99f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs2", "An observer");
	observers.push_back(observer);


	//3.:
	wrdln = new GeodeticLine(vec3(-20.0f, 10.0f, 0.0f), vec3(0.93f, 0.0f, 0.0f), "Obs1's world line");
	observer = new Observer(wrdln, "Obs3", "An observer");
	observers.push_back(observer);

	//Objects:----------------------------------------------------
	wrdln = new GeodeticLine(vec3(3.0f, -6.0f, 0.0f), vec3(0.0f, 0.99f, 0.0f), "Obj1's world line");
	objects.push_back(Object::createEarth(wrdln));

	for (int i = 0; i < 10; i++) {
		wrdln = new GeodeticLine(vec3(3.0f, -6.0f + i * 3, -1.0f), vec3(0.0f, 0.0f, 0.0f), "Obj1's world line");
		objects.push_back(Object::createEarth(wrdln));
	}

	//Dice:
	wrdln = new GeodeticLine(vec3(-10.0f, -6.0f, 0.0f), vec3(0.0f, 0.99f, 0.0f), "Obs1's world line");
	objects.push_back(Object::createDice(wrdln));
	wrdln = new GeodeticLine(vec3(-10.0f, -6.0f, 3.0f), vec3(0.0f, 0.99f, 0.0f), "Obs1's world line");
	objects.push_back(Object::createDice(wrdln));
	wrdln = new GeodeticLine(vec3(-10.0f, -6.0f, 6.0f), vec3(0.0f, 0.99f, 0.0f), "Obs1's world line");
	objects.push_back(Object::createDice(wrdln));
	for (int i = 0; i < 10; i++) {
		wrdln = new GeodeticLine(vec3(-10.0f, -6.0f + i * 3, -3.0f), vec3(0.0f, 0.0f, 0.0f), "Obj1's world line");
		objects.push_back(Object::createDice(wrdln));
		wrdln = new GeodeticLine(vec3(-10.0f, -6.0f + i * 3, 9.0f), vec3(0.0f, 0.0f, 0.0f), "Obj1's world line");
		objects.push_back(Object::createDice(wrdln));
	}


	//Background:------------------------------------------------------
	background = new Background();
	
	//Other:
	toggleActiveObserver();
}

void Scene::Control(float dt) {
	if (running) {
		dt *= timeScale;
		if (activeObserver != nullptr) {
			activeObserver->increaseTimeByDelta(dt);
		}
		//Todo
	}
}

void Scene::Animate(float dt) {
	if (running) {
		dt *= timeScale;
		if (activeObserver != nullptr) {
			activeObserver->syncCamera(realTime3DCamera);
		}
		for each (Object * obj in objects)
		{
			obj->Animate(dt);
		}
		for each (Caption * cap in captions)
		{
			cap->Animate();
		}

	}
}

void Scene::Draw(GPUProgram& gpuProgram) {
	if (activeObserver != nullptr || viewMode == diagram) {
		//Prefase:
		gpuProgram.setUniform(RelPhysics::speedOfLight, "speedOfLight");
		gpuProgram.setUniform(intersectionMode, "intersectionMode");
		gpuProgram.setUniform(dopplerMode, "dopplerMode");
		gpuProgram.setUniform(doLorentz, "doLorentz");
		gpuProgram.setUniform(doShading, "doShading");		
		gpuProgram.setUniform(viewMode, "viewMode");
		gpuProgram.setUniform(vec3(0.05, 0.05, 0.05), "La");
		activeCamera->loadOnGPU(gpuProgram);
		activeObserver->loadOnGPU(gpuProgram);
		view->Draw(gpuProgram);
	}
}

void Scene::toggleActiveObserver() {
	static int currentIdx = -1;
	if (!observers.empty()) {
		currentIdx = (observers.size() > currentIdx + 1) ? currentIdx + 1 : 0;	// Incrementation with overflow
		if (activeObserver != nullptr) {
			Observer* prevObs = activeObserver;
			activeObserver = observers.at(currentIdx);
			activeObserver->syncTimeToObserversSimultaneity(*prevObs);
		}
		else {
			activeObserver = observers.at(currentIdx);
		}
		activeObserver->syncCamera(realTime3DCamera);
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

void Scene::zoomCamera(float delta) {
	activeCamera->zoom(delta);
}


// Symulation controls:

void Scene::toggleDoppler() {
	dopplerMode = (DopplerMode)((3 > dopplerMode + 1) ? (dopplerMode + 1) : 0);
}

void Scene::toggleLorentzTransformation() {
	doLorentz = !doLorentz;
}

void Scene::toggleIntersectionType() {
	intersectionMode = (IntersectionMode)((2 > intersectionMode + 1) ? (intersectionMode + 1) : 0);
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

void Scene::toggleShading()
{
	doShading = !doShading;
}

void Scene::togglePause() {
	running = !running;
	captions.push_back(new Caption(vec2(200.0f, 200.0f), defaultFont, vec3(0.0f, 1.0f, 0.5f), "Paused"));
}

void Scene::setTime(float t) {
	if (activeObserver != nullptr) {
		activeObserver->setCurrentTimeAtAbsoluteTime(t);
	}
	bool prevState = running;
	running = true;
	Animate(0.0f);
	running = prevState;
}

void Scene::reset() {
	setTime(0.0f);
	bool prevState = running;
	running = true;
	Animate(0.0f);
	running = prevState;
}

void Scene::windTime(float deltaT) {
	activeObserver->increaseTimeByDelta(deltaT);
	bool prevState = running;
	running = true;
	Animate(0.0f);
	running = prevState;
}
