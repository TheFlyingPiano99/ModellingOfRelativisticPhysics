#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include <fstream>
#include "StringOperations.h"
#include "Exceptions.h"
#include "Assets.h"

Scene* Scene::instance = NULL;

void Scene::Initialise()
{

	hud = new HUD(this);

	//View:
	view = new RealTime3DView(this);

	//Camera:
	realTime3DCamera = new Camera();
	realTime3DCamera->initBasic(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 1.0f, 130.0f);
	diagramCamera = new Camera();
	diagramCamera->initBasic(-20 * vec3(1,1,1), vec3(0, 0, 0), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 1.0f, 130.0f);

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
	//diagramLights.push_back(new LightSource(vec3(-100, -100, -100), vec3(100000, 100000, 100000), 0));
	diagramLights.push_back(new LightSource(vec3(100, 100, 100), vec3(100000, 100000, 100000), 1));
	diagramLights.push_back(new LightSource(vec3(-1, -1, 1), vec3(1, 1, 1), 2));

	/*
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
	*/


	// Captions:-------------------------------------------------------


	// Background:-----------------------------------------------------
	background = new Background();
	
	// Coordinate system:----------------------------------------------
	system = new CoordinateSystem();

	// Load from file:-------------------------------------------------
	load("defaultSave01.txt");

	// Other:----------------------------------------------------------
	//toggleActiveObserver();
}

void Scene::Control(float dt) {
	if (loadingScene) {
		controlEvents.clear();
		if (finishedLoading) {
			finishedLoading = false;
			loadingScene = false;
			//loadThread->join();
			//delete loadThread;
			Scene::getInstance()->getHUD()->pushMessage("Scene loaded.");
		}
		return;
	}
	else if (!entryMode) {
		allowQuit = true;
		while (!controlEvents.empty()) {
			ControlEvent event = controlEvents.back();
			controlEvents.pop_back();
			switch (event) {
			case ControlEvent::toggleObserver:
				toggleActiveObserver();
				break;
			case ControlEvent::togglePause:
				togglePause();
				break;
			case ControlEvent::zoomIn:
				zoomCamera(1 - 0.01f * dt);
				break;
			case ControlEvent::zoomOut:
				zoomCamera(1 + 0.01f * dt);
				break;
			case ControlEvent::toggleDopplerEffect:
				toggleDoppler();
				break;
			case ControlEvent::rewindTime:
				windTime(-5);
				break;
			case ControlEvent::windTime:
				windTime(5);
				break;
			case ControlEvent::toggleIntersectionMode:
				toggleIntersectionMode();
				break;
			case ControlEvent::toggleLorentz:
				toggleLorentzTransformation();
				break;
			case ControlEvent::toggleViewMode:
				toggleViewMode();
				break;
			case ControlEvent::toggleShading:
				toggleShading();
				break;
			case ControlEvent::toggleSelection:
				toggleSelected();
				break;
			case ControlEvent::moveCameraForward:
				moveCamera(vec3(1, 0, 0) * cameraVelocity * dt);
				break;
			case ControlEvent::moveCameraBackward:
				moveCamera(vec3(-1, 0, 0) * cameraVelocity * dt);
				break;
			case ControlEvent::moveCameraLeft:
				moveCamera(vec3(0, -1, 0) * cameraVelocity * dt);
				break;
			case ControlEvent::moveCameraRight:
				moveCamera(vec3(0, 1, 0) * cameraVelocity * dt);
				break;
			case ControlEvent::save:
				entryMode = true;
				hud->createTextEntry("Enter save file's name!", saveHandler);
				break;
			case ControlEvent::load:
				entryMode = true;
				hud->createTextEntry("Enter file name to load!", loadHandler);
				break;
			defualt: break;
			}
		}
	}
	else {
		controlEvents.clear();		// Discard control events.
		allowQuit = false;
	}

	if (running) {
		dt *= timeScale;	// Scale time to sym speed.
		if (activeObserver != nullptr) {
			activeObserver->increaseTimeByDelta(dt);
		}
		//Todo
	}
}

void Scene::Animate(float dt) {
	if (loadingScene) {
		return;
	}
	hud->Animate(dt);						// Always animate!
	if (running) {
		dt *= timeScale;					// Scale time to symulation speed.
		if (activeObserver != nullptr) {
			activeObserver->syncCamera(realTime3DCamera);
		}
		for each (Object * obj in objects)
		{
			obj->Animate(dt);
		}
	}
}

void Scene::Draw(GPUProgram& gpuProgram) {
	if (loadingScene) {
		return;
	}
	if (activeObserver != nullptr || viewMode == diagram) {
		//Prefase:
		gpuProgram.setUniform(false, "textMode");
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
		hud->pushMessage(std::string("Active observer: ").append(activeObserver->getName()).c_str());
	}
}

void Scene::type(char c)
{
	bool result;
	if (entryMode) {
		result = hud->type(c);
		if (result) {
			entryMode = false;
		}
	}
}

// Camera controls:

void Scene::panCamera(float cx, float cy) {
	static float camSpeed = 0.01f;
	if (viewMode == realTime3D) {
		realTime3DCamera->rotateAroundEye(cx * realTime3DCamera->getAspectRatio(), -cy);
	}
	else if (viewMode == diagram) {
		diagramCamera->rotateAroundLookat(cx * diagramCamera->getAspectRatio(), -cy);
	}
}

void Scene::zoomCamera(float delta) {
	activeCamera->zoom(delta);
}

void Scene::moveCamera(vec3 delta)
{
	if (viewMode == ViewMode::diagram) {
		activeCamera->move(delta);
	}
}


// Symulation controls:

void Scene::toggleDoppler() {
	dopplerMode = (DopplerMode)((3 > dopplerMode + 1) ? (dopplerMode + 1) : 0);
	std::string str("Doppler mode: ");
	switch (dopplerMode)
	{
	case DopplerMode::full:
		str.append("full");
		break;
	case DopplerMode::mild:
		str.append("mild");
		break;
	case DopplerMode::off:
		str.append("off");
		break;
	default:
		break;
	}
	hud->pushMessage(str.c_str());
}

void Scene::toggleLorentzTransformation() {
	doLorentz = !doLorentz;
	std::string str;
	if (doLorentz) {
		str = std::string("Lorentz transformation enabled");
	}
	else {
		str = std::string("Lorentz transformation disabled");
	}
	hud->pushMessage(str.c_str());
}

void Scene::toggleIntersectionMode() {
	intersectionMode = (IntersectionMode)((2 > intersectionMode + 1) ? (intersectionMode + 1) : 0);
	std::string str("Intersection mode: ");
	switch (intersectionMode)
	{
	case IntersectionMode::lightCone:
		str.append("light cone");
		break;
	case IntersectionMode::hyperplane:
		str.append("simultaneous hyperplane");
		break;
	default:
		break;
	}
	hud->pushMessage(str.c_str());
}

void Scene::toggleViewMode() {
	viewMode = (ViewMode)((2 > viewMode + 1) ? (viewMode + 1) : 0);
	std::string str;
	switch (viewMode)
	{
	case ViewMode::realTime3D:
		activeCamera = realTime3DCamera;
		view->transitionFrom();
		delete view;
		view = new RealTime3DView(this);
		view->transitionTo();
		str = std::string("View mode: real time 3D");
		break;
	case ViewMode::diagram:
		activeCamera = diagramCamera;
		view->transitionFrom();
		delete view;
		view = new DiagramView(this);
		view->transitionTo();
		str = std::string("View mode: diagram (x, y, t)");
		break;
	default:
		break;
	}
	hud->pushMessage(str.c_str());
}

void Scene::toggleShading()
{
	doShading = !doShading;
	std::string str;
	if (doShading) {
		str = std::string("Shading enabled");
	}
	else {
		str = std::string("Shading disabled");
	}
	hud->pushMessage(str.c_str());
}

void Scene::togglePause() {
	running = !running;
	std::string str;
	if (running) {
		str = std::string("Resumed");
	}
	else {
		str = std::string("Paused");
	}
	hud->pushMessage(str.c_str());
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
	hud->pushMessage("Reset");
}

void Scene::windTime(float deltaTau) {
	float realDelta = activeObserver->increaseTimeByDelta(deltaTau);
	bool prevState = running;
	running = true;
	Animate(0.0f);
	running = prevState;
	std::string str("Time shifted by tau = ");
	hud->pushMessage(str.append(std::to_string(realDelta)).append(" m").c_str());
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
		hud->pushMessage(std::string("Selected object: ")
			.append(selected->getName())
			.append(" ")
			.append(std::to_string(selected->getID()))
			.c_str());
	}
}

void Scene::selectByClick(float cX, float cY)
{
	Entity* entity = getUnderCursor(cX, cY);
	if (entity != nullptr) {
		if (selected != nullptr) {
			selected->deselect();
		}
		selected = entity;
		selected->select();
	}
}

void Scene::mouseMoved(float cX, float cY)
{
	Entity* entity = getUnderCursor(cX, cY);
	if (entity != hovered) {
		if (hovered != nullptr) {
			hovered->endHover();
		}
		if (entity != nullptr) {
			hovered = entity;
			hovered->hover();
		}
		else {
			hovered = nullptr;
		}
	}
}

Entity* Scene::getUnderCursor(float cX, float cY)
{
	if (!objects.empty()) {		// There are objects in the scene.
		Ray ray = activeCamera->getRayFromCameraCoord(vec2(cX, cY));

		int selectionIdx = -1;			// index of the selected object
		if (viewMode == ViewMode::diagram) {								// Diagram view
			float constraint = 0.2f;
			float shortestDistance = objects[0]->rayDistanceToDiagram(ray,
				activeObserver->getStartPos(),
				activeObserver->getVelocity(),
				diagramX, diagramY, diagramZ);		// First item handled separately.
			if (constraint > shortestDistance && shortestDistance > 0) {
				selectionIdx = 0;
			}
			for (int i = 1; i < objects.size(); i++) {
				float d = objects[i]->rayDistanceToDiagram(ray,
					activeObserver->getStartPos(),
					activeObserver->getVelocity(),
					diagramX, diagramY, diagramZ);
				if ((shortestDistance < 0 || shortestDistance > d) && d < constraint && d > 0) {
					shortestDistance = d;
					selectionIdx = i;
				}
			}
		}
		else if (viewMode == ViewMode::realTime3D) {						// RealTime3D view
			Intersectable* intersectable;
			if (intersectionMode == IntersectionMode::lightCone) {
				intersectable = activeObserver->getLightCone();
			}
			else if (intersectionMode == IntersectionMode::hyperplane) {
				intersectable = activeObserver->getHyperplane();
			}
			// First item handled separately:
			float shortestDistance = objects[0]->rayDistanceToObject(ray, intersectable, doLorentz, activeObserver->getLocation(), activeObserver->getStartPos(), activeObserver->getVelocity());
			if (objects[0]->getOverallRadius() > shortestDistance && shortestDistance > 0) {
				selectionIdx = 0;
			}
			for (int i = 1; i < objects.size(); i++) {
				float d = objects[i]->rayDistanceToObject(ray, intersectable, doLorentz, activeObserver->getLocation(), activeObserver->getStartPos(), activeObserver->getVelocity());
				if ((shortestDistance < 0 || shortestDistance > d) && d < objects[i]->getOverallRadius() && d > 0) {
					shortestDistance = d;
					selectionIdx = i;
				}
			}
			delete intersectable;
		}

		if (selectionIdx >= 0) {
			return objects[selectionIdx];
		}
	}
	return nullptr;
}

void Scene::save(const char* destinationFile)
{
	entryMode = false;

	std::ofstream file;
	file.open(Assets::getSavesPath().append(destinationFile));
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
		hud->pushMessage("Scene saved.");
	}
	else {
		hud->pushMessage("Cannot save scene!");
	}
}

void loadThreadFunction(std::ifstream* file)
{
}


std::ifstream* file = NULL;	// Param for thread function.

void Scene::load(const char* sourceFileName)
{
	file = new std::ifstream();		// Deleted in thread function!
	file->open(Assets::getSavesPath().append(sourceFileName));
	if (file->is_open()) {
		// Clear everything, what will be loaded:

		//Scene::getInstance()->pause();
		Scene::getInstance()->clearScene();

		entryMode = false;
		loadingScene = true;
		finishedLoading = false;
		//loadThread = new std::thread(loadThreadFunction, file);
		std::map<int, WorldLine*> worldLines;	// map<ID, pointer>
		std::string line;
		while (getline(*file, line)) {
			std::vector<std::string> words;
			words = split(line, ' ');
			if (words.empty()) {									// Empty line
				continue;
			}
			else if (words.at(0).at(0) == '#') {					// Comment
				continue;
			}
			else if (words.at(0).compare("GeodeticLine") == 0) {	// GeodeticLine
				WorldLine* wrdLn = GeodeticLine::loadFromFile(*file);
				if (wrdLn != nullptr) {
					worldLines.insert({ wrdLn->getID(), wrdLn });
				}
			}
			else if (words.at(0).compare("Observer") == 0) {		// Observer
				Observer* obs = Observer::loadFromFile(*file);
				if (obs != nullptr) {
					Scene::getInstance()->getObservers()->push_back(obs);
				}
			}
			else if (words.at(0).compare("Object") == 0) {		// Object
				Object* obj = Object::loadFromFile(*file);
				if (obj != nullptr) {
					Scene::getInstance()->getObjects()->push_back(obj);
				}
			}
		}

		// Linking objects (WorldLines to Obserers and Objects):
		for (const auto& obs : *(Scene::getInstance()->getObservers())) {
			obs->setWorldLine(worldLines);
		}
		for (const auto& obj : *(Scene::getInstance()->getObjects())) {
			obj->setWorldLine(worldLines);
		}

		Scene::getInstance()->toggleActiveObserver();
		file->close();
		delete file;
		Scene::getInstance()->setFinishedLoading(true);
	}
	else {
		Scene::getInstance()->getHUD()->pushMessage("Cannot load scene!");
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
	hud->clearCaptions();
}

void Scene::pause()
{
	running = false;
	hud->pushMessage("Paused");
}

void Scene::resume()
{
	running = true;
	hud->pushMessage("Resumed");
}


//Handlers:--------------------------------------------------------------

void saveHandler(const char* str)
{
	Scene::getInstance()->save(str);
}

void loadHandler(const char* str)
{
	Scene::getInstance()->load(str);
}
