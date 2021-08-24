#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include <fstream>
#include "StringOperations.h"
#include "Exceptions.h"

void Scene::Initialise()
{
	//View:
	view = new RealTime3DView(this);

	//Camera:
	realTime3DCamera = new Camera();
	realTime3DCamera->initBasic(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);
	diagramCamera = new Camera();
	diagramCamera->initBasic(20 * vec3(1,1,1), 19 * vec3(1, 1, 1), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

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
	wrdln = new GeodeticLine(vec3(0.0f, -6.0f, 0.0f), vec3(0.0f, 0.99f, 0.0f), "Obs2's world line");
	observer = new Observer(wrdln, "Obs2", "An observer");
	observers.push_back(observer);

	//3.:
	wrdln = new GeodeticLine(vec3(-20.0f, 10.0f, 0.0f), vec3(0.93f, 0.0f, 0.0f), "Obs3's world line");
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
	wrdln = new GeodeticLine(vec3(-10.0f, -6.0f, 0.0f), vec3(0.0f, 0.99f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	wrdln = new GeodeticLine(vec3(-10.0f, -6.0f, 3.0f), vec3(0.0f, 0.99f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	wrdln = new GeodeticLine(vec3(-10.0f, -6.0f, 6.0f), vec3(0.0f, 0.99f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	for (int i = 0; i < 10; i++) {
		wrdln = new GeodeticLine(vec3(-10.0f, -6.0f + i * 3, -3.0f), vec3(0.0f, 0.0f, 0.0f), "");
		objects.push_back(Object::createDice(wrdln));
		wrdln = new GeodeticLine(vec3(-10.0f, -6.0f + i * 3, 9.0f), vec3(0.0f, 0.0f, 0.0f), "");
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
		if (activeObserver != nullptr) {
			currentIdx = (observers.size() > currentIdx + 1) ? currentIdx + 1 : 0;	// Incrementation with overflow
			Observer* prevObs = activeObserver;
			activeObserver = observers.at(currentIdx);
			activeObserver->syncTimeToObserversSimultaneity(*prevObs);
		}
		else {
			currentIdx = 0;
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

void Scene::toggleSelected()
{
	static int currentIdx = -1;
	if (!objects.empty()) {
		if (selected != nullptr) {
			currentIdx = (objects.size() > currentIdx + 1) ? currentIdx + 1 : 0;	// Incrementation with overflow
			selected->deselect();
		}
		else {
			currentIdx = 0;
		}
		selected = objects[currentIdx];
		selected->select();
	}
}

void Scene::selectByClick(float cX, float cY)
{
	if (!objects.empty()) {		// There are objects in the scene.
		Ray ray = activeCamera->getRayFromCameraCoord(vec2(cX, cY));

		//For testing:
		vec4 eye = vec4(activeCamera->getEye().x, activeCamera->getEye().y, 0, activeCamera->getEye().z);
		WorldLine* line = new GeodeticLine(vec4(0,0,0,0), vec4(ray.dir.x, ray.dir.y, 0, ray.dir.z), "Click ray", "");
		linesToDisplay.push_back(line);


		int selectionIdx = -1;			// index of the selected object
		if (viewMode == ViewMode::diagram) {								// Diagram view
			float constraint = 0.5f;
			float shortestDistance = objects[0]->rayDistanceToDiagram(ray);		// First item handled separately.
			if (constraint > shortestDistance && shortestDistance > 0) {
				selectionIdx = 0;
			}
			for (int i = 1; i < objects.size(); i++) {
				float d = objects[i]->rayDistanceToDiagram(ray);
				if ((shortestDistance < 0 || shortestDistance > d) && d < constraint && d > 0) {
					shortestDistance = d;
					selectionIdx = i;
				}
			}
		}
		else if (viewMode == ViewMode::realTime3D) {						// RealTime3D view
			float constraint = 1.0f;		// greater constraint
			Intersectable* intersectable;
			if (intersectionMode == IntersectionMode::lightCone) {
				intersectable = activeObserver->getLightCone();
			}
			else if (intersectionMode == IntersectionMode::hyperplane) {
				intersectable = activeObserver->getHyperplane();
			}
			// First item handled separately:
			float shortestDistance = objects[0]->rayDistanceToObject(ray, intersectionMode, intersectable, doLorentz, activeObserver->getLocation(), activeObserver->getStartPos(), activeObserver->getVelocity());
			if (constraint > shortestDistance && shortestDistance > 0) {
				selectionIdx = 0;
			}
			for (int i = 1; i < objects.size(); i++) {
				float d = objects[i]->rayDistanceToObject(ray, intersectionMode, intersectable, doLorentz, activeObserver->getLocation(), activeObserver->getStartPos(), activeObserver->getVelocity());
				if ( (shortestDistance < 0 || shortestDistance > d) && d < constraint && d > 0) {
					shortestDistance = d;
					selectionIdx = i;
				}
			}
			delete intersectable;
		}

		if (selectionIdx >= 0) {
			if (selected != nullptr) {
				selected->deselect();
			}
			selected = objects[selectionIdx];
			selected->select();
		}
	}
}

void Scene::save(const char* destination)
{
	std::ofstream file;
	file.open(destination);
	if (file.is_open()) {
		file.clear();
/*
		for (const auto& [id, wl] : worldLines)
		{
			file << wl->genSaveString() << std::endl;
		}
*/
		for each (Observer * obs in observers)
		{
			file << obs->genSaveString() << std::endl;
		}
		for each (Object * obj in objects)
		{
			file << obj->genSaveString() << std::endl;
		}
		file.close();
	}
}

void Scene::load(const char* source)
{
	std::ifstream file;
	file.open(source);
	if (file.is_open()) {
		// Clear everything, what will be loaded:
		
		pause();
		clearScene();

		std::map<int, WorldLine*> worldLines;	// map<ID, pointer>
		std::string line;
		while (getline(file, line)) {
			std::vector<std::string> words;
			words = split(line, ' ');
			if (words.empty()) {									// Empty line
				continue;
			}
			else if (words.at(0).at(0) == '#') {					// Comment
				continue;
			}
			else if (words.at(0).compare("GeodeticLine") == 0) {	// GeodeticLine
				 WorldLine* wrdLn = GeodeticLine::loadFromFile(file);
				 if (wrdLn != nullptr) {
					 worldLines.insert({ wrdLn->getID(), wrdLn });
				 }
			}
			else if (words.at(0).compare("Observer") == 0) {		// Observer
				Observer* obs = Observer::loadFromFile(file);
				if (obs != nullptr) {
					observers.push_back(obs);
				}
			}
			else if (words.at(0).compare("Object") == 0) {		// Object
				Object* obj = Object::loadFromFile(file);
				if (obj != nullptr) {
					objects.push_back(obj);
				}
			}
		}

		// Linking objects (WorldLines to Obserers and Objects):
		for (const auto& obs : observers) {
			obs->setWorldLine(worldLines);
		}
		for (const auto& obj : objects) {
			obj->setWorldLine(worldLines);
		}

		toggleActiveObserver();
		file.close();
	}
	else {
		throw CannotLoadScene();
	}
}

void Scene::clearScene()
{
	for (const auto& obs : observers) {
		delete obs;
	}
	for (const auto& obj : objects) {
		delete obj;
	}
	observers.clear();
	objects.clear();
	activeObserver = NULL;
	selected = NULL;
}

void Scene::pause()
{
	running = false;
}

void Scene::resume()
{
	running = true;
}
