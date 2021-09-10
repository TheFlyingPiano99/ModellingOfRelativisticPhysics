#pragma once

#include <vector>
#include <iostream>

#include "Camera.h"
#include "Observer.h"
#include "Object.h"
#include "Background.h"
#include "Caption.h"
#include "MenuSystem.h"
#include "LightSource.h"
#include "View.h"
#include "EnumTypes.h"
#include "CoordinateSystem.h"
#include "HUD.h"
#include "ControlEventInterface.h"

#include <map>
#include <thread>

class Scene {
	static Scene* instance;
	float timeScale = 0.002f; // Time scale of the simulation. (1 reallife millisec = to "timeScale" * 1[m])
	float cameraVelocity = 1;
	View* view;
	Camera* realTime3DCamera = NULL;
	Camera* diagramCamera = NULL;
	Camera* activeCamera = NULL;
	CoordinateSystem* system = NULL;
	HUD* hud = NULL;

	Observer* activeObserver = NULL;
	Entity* selected = NULL;
	Entity* hovered = NULL;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	std::vector<LightSource*> diagramLights;
	std::vector<WorldLine*> linesToDisplay;				// For testing only!!!
	std::vector<ControlEventInterface*> controlEvents;

	Background* background;

	MenuSystem menu;
	bool entryMode = false;
	bool allowQuit = false;
	bool loadingScene = false;
	bool finishedLoading = false;

	Settings settings;		// All important settings mainly related to rendering.

	//std::thread* loadThread;

	Scene() {}

public:

	static Scene* getInstance() {
		if (instance == nullptr) {
			instance = new Scene();
		}
		return instance;
	}

	~Scene() {
		delete view;
		for each (Observer* obs in observers)
		{
			delete obs;
		}
		for each (Object * obj in objects)
		{
			delete obj;
		}
		for each (LightSource * lt in lights)
		{
			delete lt;
		}
		for each (LightSource * lt in diagramLights)
		{
			delete lt;
		}

		for each (WorldLine* line in linesToDisplay)
		{
			delete line;
		}

		delete background;
		delete system;
		delete hud;
		/*
		if (loadThread != nullptr && loadThread->joinable()) {
			loadThread->join();
		}
		delete loadThread;
		*/

	}

	/*
	* Called once, after the app is launched.
	*/
	void Initialise();

	void Control(float dt);

	void Animate(float dt);

	/*
	* Renders everything on screen.
	*/
	void Draw(GPUProgram& gpuProgram);

	void toggleActiveObserver();

	// Typing
	void type(char c);

	// Camera controls:
	void panCamera(float cx, float cy);

	void zoomCamera(float delta);

	void moveCamera(vec3 delta);

	// Symulation controls:
	void toggleDoppler();

	void toggleLorentzTransformation();

	void toggleTransformToProperFrame();

	/*
	* Switches between simultaneous hyperplane intersection and light cone intersection.
	*/
	void toggleIntersectionMode();

	void toggleViewMode();

	void toggleShading();


	// Time manipulation:

	void togglePause();

	void setTime(float t);

	void reset();

	void windTime(float deltaTau);

	// Save / load control:

	void startSaveProcess();

	void startLoadProcess();

	// Selection:

	void toggleSelected();

	/*
	* Select object, where the mouse clicked
	* Receives X, and Y coordinates in camera space... [-1; 1]
	*/
	void selectByClick(float cX, float cY);

	void mouseMoved(float cX, float cY);

	Entity* getUnderCursor(float cX, float cY);

	// Getters:--------------------------------------------------------------------

	std::vector<LightSource*>* getLights() {
		return &lights;
	}

	Background* getBackground() {
		return background;
	}

	Camera* getActiveCamera() {
		return activeCamera;
	}

	std::vector<Object*>* getObjects() {
		return &objects;
	}

	std::vector<LightSource*>* getDiagramLights() {
		return &diagramLights;
	}

	std::vector<Observer*>* getObservers() {
		return &observers;
	}

	Observer* getActiveObserver() {
		return activeObserver;
	}

	Settings& getSettings() {
		return settings;
	}

	CoordinateSystem* getCoordinateSystem() {
		return system;
	}

	HUD* getHUD() {
		return hud;
	}

	float getCameraVelocity() {
		return cameraVelocity;
	}

	//-----------------------------------------------------------------

	/*
	* Saves scene.
	*/
	void save(const char* destination);

	/*
	* Loads scene.
	*/
	void load(const char* source);

	/*
	* Removes all Observers and Objects from the scene.
	*/
	void clearScene();

	void pause();
	void resume();



	/*
	* Returns true if quiting is permitted.
	*/
	bool askToQuit() {
		return allowQuit;
	}

	//For testing:
	std::vector<WorldLine*>& getLines() {
		return linesToDisplay;
	}

	void pushBackControlEvent(ControlEventInterface* e) {
		controlEvents.push_back(e);
	}

	void setFinishedLoading(bool b) {
		finishedLoading = b;
	}

};


void saveHandler(const char* str);

void loadHandler(const char* str);
