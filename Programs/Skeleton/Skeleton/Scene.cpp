#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include <fstream>
#include "StringOperations.h"
#include "Exceptions.h"
#include "Assets.h"
#include "Plane.h"

Scene* Scene::instance = NULL;

void Scene::Initialise()
{
	settings.viewMode = ViewMode::realTime3D;
	settings.intersectionMode.get() = IntersectionMode::lightCone;
	settings.dopplerMode.get() = DopplerMode::mild;
	settings.doLorentz.get() = true;
	settings.transformToProperFrame.get() = true;
	settings.doShading = true;
	settings.running = false;
	settings.diagramX = 0;
	settings.diagramY = 1;
	settings.diagramZ = 3;
	settings.diagramNotVisualised = 2;

	hud = new HUD(this);
	editor = new Editor(this);

	//View:
	view = new RealTime3DView(this);

	//Camera:
	realTime3DCamera = new Camera();
	realTime3DCamera->initBasic(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 1.0f, 130.0f);
	diagramCamera = new Camera();
	diagramCamera->initBasic(-20 * vec3(1,1,1), vec3(0, 0, 0), vec3(0.0f, 0.0f, 1.0f), (M_PI / 2.0f), (float)windowWidth / (float)windowHeight, 1.0f, 130.0f);

	if (settings.viewMode == realTime3D) {
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
	wrdln = new CompositeLine(vec3(-10.0f, -6.0f, 0.0f), vec3(0.0f, 0.50f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	wrdln = new CompositeLine(vec3(-10.0f, -6.0f, 3.0f), vec3(0.0f, 0.5f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	wrdln = new CompositeLine(vec3(-10.0f, -6.0f, 6.0f), vec3(0.0f, 0.50f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	for (int i = 0; i < 10; i++) {
		wrdln = new GeodeticLine(vec3(-10.0f, -6.0f + i * 3, -3.0f), vec3(0.0f, 0.0f, 0.0f), "");
		objects.push_back(Object::createDice(wrdln));
		wrdln = new GeodeticLine(vec3(-10.0f, -6.0f + i * 3, 9.0f), vec3(0.0f, 0.0f, 0.0f), "");
		objects.push_back(Object::createDice(wrdln));
	}

	objects.push_back(Object::createSpaceship(new CompositeLine(vec3(10.0f, 10.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), "")));
	objects.push_back(Object::createSpaceship(new CompositeLine(vec3(10.0f, 12.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), "")));
	*/


	// Captions:-------------------------------------------------------

	// Load from file:-------------------------------------------------
	load("defaultSave01.txt");
	/*
	WorldLine* wrdln = new GeodeticLine(vec3(-10.0f, -9.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "");
	observers.push_back(new Observer(wrdln));
	wrdln = new CompositeLine(vec3(-10.0f, -9.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "");
	objects.push_back(Object::createDice(wrdln));
	*/

	// Background:-----------------------------------------------------
	background = new Background();

	// Other:----------------------------------------------------------
	//toggleActiveObserver();
	hud->updateSettings(settings);
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
			ControlEventInterface* event = controlEvents.back();
			controlEvents.pop_back();
			event->performAction(dt);
		}
	}
	else {
		controlEvents.clear();		// Discard control events.
		allowQuit = false;
	}

	if (settings.running) {
		dt *= timeScale;	// Scale time to sym speed.
		if (activeObserver != nullptr) {
			activeObserver->changeTimeByDelta(dt, settings);
		}
		//Todo
	}
}

void Scene::Animate(float dt) {
	if (loadingScene) {
		return;
	}
	hud->Animate(dt);						// Always animate!
	settings.intersectionMode.animate(dt);
	settings.dopplerMode.animate(dt);
	settings.doLorentz.animate(dt);
	settings.transformToProperFrame.animate(dt);

	// "In app world time" animate
	if (settings.running) {

		dt *= timeScale;					// Scale time to symulation speed.
		if (activeObserver != nullptr) {
			activeObserver->syncCamera(realTime3DCamera, settings);
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
	if (activeObserver != nullptr) {
		//Prefase:
		RelPhysics::loadOnGPU(gpuProgram);
		settings.loadOnGPU(gpuProgram);
		activeCamera->loadOnGPU(gpuProgram);
		activeObserver->loadOnGPU(gpuProgram, settings);
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
			activeObserver->syncTimeToObserversSimultaneity(*prevObs, settings);
		}
		else {
			currentIdx = 0;
			activeObserver = observers.at(currentIdx);
		}
		activeObserver->syncCamera(realTime3DCamera, settings);
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
	if (settings.viewMode == realTime3D) {
		realTime3DCamera->rotateAroundEye(cx * realTime3DCamera->getAspectRatio(), -cy);
	}
	else if (settings.viewMode == diagram) {
		diagramCamera->rotateAroundLookat(cx * diagramCamera->getAspectRatio(), -cy);
	}
}

void Scene::zoomCamera(float delta) {
	activeCamera->zoom(delta);
}

void Scene::moveCamera(vec3 delta)
{
	if (settings.viewMode == ViewMode::diagram) {
		activeCamera->move(delta);
	}
}


// Symulation controls:

void Scene::toggleDoppler() {
	settings.dopplerMode = (DopplerMode)((3 > settings.dopplerMode.get() + 1) ? (settings.dopplerMode.get() + 1) : 0);
	std::string str("Doppler mode: ");
	switch (settings.dopplerMode.get())
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
	settings.doLorentz.get() = !settings.doLorentz.get();
	std::string str;
	if (settings.doLorentz.get()) {
		str = std::string("Lorentz transformation enabled");
	}
	else {
		str = std::string("Lorentz transformation disabled");
	}
	hud->pushMessage(str.c_str());
	hud->updateSettings(settings);
}

void Scene::toggleTransformToProperFrame()
{
	if (settings.editorMode) {	// In editor mode no transformation is allowed!
		settings.transformToProperFrame.get() = false;
		return;
	}
	settings.transformToProperFrame.get() = !settings.transformToProperFrame.get();
	std::string str;
	if (settings.transformToProperFrame.get()) {
		str = std::string("Diagram transformed to active observers frame");
	}
	else {
		str = std::string("Diagram transformed to absolute observers frame");
	}
	hud->pushMessage(str.c_str());
	hud->updateSettings(settings);
}

void Scene::toggleIntersectionMode() {
	settings.intersectionMode = (IntersectionMode)((2 > settings.intersectionMode.get() + 1) ? (settings.intersectionMode.get() + 1) : 0);
	std::string str("Intersection mode: ");
	switch (settings.intersectionMode.get())
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
	hud->updateSettings(settings);
}

void Scene::toggleViewMode() {
	settings.viewMode = (ViewMode)((2 > settings.viewMode + 1) ? (settings.viewMode + 1) : 0);
	std::string str;
	switch (settings.viewMode)
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
		settings.editorMode = false;		// Turn off editor mode!
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
	hud->updateSettings(settings);
}

void Scene::toggleShading()
{
	settings.doShading = !settings.doShading;
	std::string str;
	if (settings.doShading) {
		str = std::string("Shading enabled");
	}
	else {
		str = std::string("Shading disabled");
	}
	hud->pushMessage(str.c_str());
}

void Scene::togglePause() {
	settings.running = !settings.running;
	std::string str;
	if (settings.running) {
		str = std::string("Resumed");
	}
	else {
		str = std::string("Paused");
	}
	hud->pushMessage(str.c_str());
	hud->updateSettings(settings);
}

void Scene::setTime(float t) {
	if (activeObserver != nullptr) {
		activeObserver->setCurrentTimeAtAbsoluteTime(t, settings);
	}
	bool prevState = settings.running;
	settings.running = true;
	Animate(0.0f);
	settings.running = prevState;
}

void Scene::reset() {
	setTime(0.0f);
	bool prevState = settings.running;
	settings.running = true;
	Animate(0.0f);
	settings.running = prevState;
	hud->pushMessage("Reset");
}

void Scene::windTime(float deltaTau) {
	float realDelta = activeObserver->changeTimeByDelta(deltaTau, settings);
	bool prevState = settings.running;
	settings.running = true;
	Animate(0.0f);
	settings.running = prevState;
	std::string str("Time shifted by tau = ");
	hud->pushMessage(str.append(std::to_string(realDelta)).append(" m").c_str());
}

void Scene::startSaveProcess()
{
	entryMode = true;
	hud->createTextEntry("Enter save file's name!", saveHandler);
}

void Scene::startLoadProcess()
{
	entryMode = true;
	hud->createTextEntry("Enter file name to load!", loadHandler);
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
		grabbed = selected;
	}
}

void Scene::releaseGrab()
{
	grabbed = NULL;
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

void Scene::mouseDragged(const float cX, const float cY, const float deltaCX, const float deltaCY, const MouseState& mouseState)
{
	if (mouseState.mouseRightDown) {
		panCamera(deltaCX, deltaCY);
		return;
	}
	else if (mouseState.mouseLeftDown) {
		if (settings.viewMode == ViewMode::diagram && settings.editorMode && grabbed != nullptr) {
			grabbed->draggedTo(getEditedLocation(cX, cY));
		}
	}
}

Entity* Scene::getUnderCursor(float cX, float cY)
{
	if (!objects.empty()) {		// There are objects in the scene.
		Ray ray = activeCamera->getRayFromCameraCoord(vec2(cX, cY));

		int selectionIdx = -1;			// index of the selected object
		if (settings.viewMode == ViewMode::diagram) {								// Diagram view
			float constraint = 0.2f;
			float shortestDistance = objects[0]->rayDistanceToDiagram(ray,
				activeObserver->getProperties(settings),
				settings);		// First item handled separately.
			if (constraint > shortestDistance && shortestDistance > 0) {
				selectionIdx = 0;
			}
			for (int i = 1; i < objects.size(); i++) {
				float d = objects[i]->rayDistanceToDiagram(ray,
					activeObserver->getProperties(settings),
					settings);
				if ((shortestDistance < 0 || shortestDistance > d) && d < constraint && d > 0) {
					shortestDistance = d;
					selectionIdx = i;
				}
			}
		}
		else if (settings.viewMode == ViewMode::realTime3D) {						// RealTime3D view
			LightCone* lightCone = activeObserver->getLightCone(settings);
			Hyperplane* hyperplane = activeObserver->getHyperplane(settings);
			// First item handled separately:
			float shortestDistance = objects[0]->rayDistanceToObject(ray, *lightCone, *hyperplane, settings, activeObserver->getLocation(settings), activeObserver->getLocationAtZero(settings), activeObserver->getVelocity(settings));
			if (objects[0]->getOverallRadius() > shortestDistance && shortestDistance > 0) {
				selectionIdx = 0;
			}
			for (int i = 1; i < objects.size(); i++) {
				float d = objects[i]->rayDistanceToObject(ray, *lightCone, *hyperplane, settings, activeObserver->getLocation(settings), activeObserver->getLocationAtZero(settings), activeObserver->getVelocity(settings));
				if ((shortestDistance < 0 || shortestDistance > d) && d < objects[i]->getOverallRadius() && d > 0) {
					shortestDistance = d;
					selectionIdx = i;
				}
			}
			delete lightCone;
			delete hyperplane;
		}

		if (selectionIdx >= 0) {
			return objects[selectionIdx];
		}
	}
	return nullptr;
}

void Scene::toggleEditorMode()
{
	if (settings.viewMode != ViewMode::diagram) {	// Toggle only in diagram mode.
		return;
	}
	settings.editorMode = !settings.editorMode;
	if (settings.editorMode) {
		settings.transformToProperFrame.get() = false;
		hud->pushMessage("Editor mode on");
	}
	else {
		hud->pushMessage("Editor mode off");
	}
	hud->updateSettings(settings);
}

void Scene::deleteSelected()
{
	if (settings.editorMode && selected != nullptr) {
		int i = 0;
		for (; i < objects.size(); i++) {
			if (objects[i] == selected) {
				break;
			}
		}
		objects.erase(objects.begin() + i);
		delete selected;
	}
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
		
		// Set flags:
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
			else if (words.at(0).compare("CompositeLine") == 0) {	// CompositeLine
				WorldLine* wrdLn = CompositeLine::loadFromFile(*file);
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

		delete system;
		system = new CoordinateSystem();
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
	grabbed = NULL;
	hud->clearCaptions();
}

void Scene::pause()
{
	settings.running = false;
	hud->pushMessage("Paused");
	hud->updateSettings(settings);
}

void Scene::resume()
{
	settings.running = true;
	hud->pushMessage("Resumed");
	hud->updateSettings(settings);
}

vec4 Scene::getEditedLocation(const float cX, const float cY)
{
	Ray ray = activeCamera->getRayFromCameraCoord(vec2(cX, cY));
	ObserverProperties properties;
	if (settings.viewMode == ViewMode::diagram && settings.transformToProperFrame.get()) {
		properties = activeObserver->getProperties(settings);
	}
	else {
		properties = RelPhysics::getAbsulteObserverProperties();
	}

	if (grabbed == nullptr) {							// Pre condition
		throw std::exception("Nothing is grabbed!");
	}

	vec4 referenceLocation = grabbed->getClosestLocation(ray, properties, settings);
	vec3 planePos;
	if (settings.viewMode == ViewMode::diagram) {
		planePos = vec3(referenceLocation[settings.diagramX], referenceLocation[settings.diagramY], referenceLocation[settings.diagramZ]);
	}
	else if (settings.viewMode == ViewMode::realTime3D) {
		throw std::exception("Unimplemented behaviour!");
	}
	vec3 prefUp = activeCamera->getPrefUp();
	vec3 right = activeCamera->getRight();
	vec3 planeNorm = cross(right, prefUp);
	Plane plane = Plane(planePos, planeNorm);
	vec3 diagramPos = intersect(plane, ray);
	vec4 location;
	location[settings.diagramX] = diagramPos.x;
	location[settings.diagramY] = diagramPos.y;
	location[settings.diagramZ] = diagramPos.z;
	location[settings.diagramNotVisualised] = referenceLocation[settings.diagramNotVisualised];	// Do not change!

	// Return to absolute frame:
	vec4 invTransformed = interpolate(settings.transformToProperFrame, true, false,
		interpolate(settings.doLorentz, true, false,
			RelPhysics::lorentzTransformation(location,
				RelPhysics::lorentzTransformationOfVelocity(vec3(0, 0, 0),
					RelPhysics::To3DVelocity(activeObserver->getVelocity(settings)))),
			RelPhysics::galileanTransformation(location,
				RelPhysics::galileanTransformationOfVelocity(vec3(0, 0, 0),
					RelPhysics::To3DVelocity(activeObserver->getVelocity(settings))))
			),
		location
	);
	invTransformed[settings.diagramNotVisualised] = referenceLocation[settings.diagramNotVisualised];
	return invTransformed;
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
