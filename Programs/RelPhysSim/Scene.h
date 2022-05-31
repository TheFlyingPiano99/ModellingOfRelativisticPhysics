#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <thread>

#include "Camera.h"
#include "Observer.h"
#include "Object.h"
#include "Background.h"
#include "Caption.h"
#include "LightSource.h"
#include "View.h"
#include "EnumTypes.h"
#include "CoordinateSystem.h"
#include "HUD.h"
#include "IControlEvent.h"
#include "Editor.h"
#include "Initialiser.h"

enum CreationSequence {
	null,
	costumize
};

class Scene {
	static Scene* instance;
	float timeScale = 0.005f; // Time scale of the simulation. (1 reallife millisec = to "timeScale" * 1[m])
	float cameraVelocity = 1;
	View* view;
	Camera* realTime3DCamera = nullptr;
	Camera* diagramCamera = nullptr;
	Camera* activeCamera = nullptr;
	CoordinateSystem* system = nullptr;
	HUD* hud = nullptr;
	Editor* editor = nullptr;

	Observer* activeObserver = nullptr;
	Entity* selected = nullptr;
	Entity* grabbed = nullptr;
	Entity* hovered = nullptr;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	std::vector<LightSource*> diagramLights;
	std::vector<WorldLine*> linesToDisplay;				// For testing only!!!
	std::vector<IControlEvent*> controlEvents;

	Background* background;
	vec3 backgroundColor;

	bool entryMode = false;
	bool allowQuit = false;
	bool loadingScene = false;
	bool finishedLoading = false;
	CreationSequence creationSequence = CreationSequence::null;
	Initialiser* initialiser = nullptr;

	RelTypes::Settings settings;		// All important settings mainly related to rendering.


	Scene() = default;

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
		if (initialiser != nullptr) {
			delete initialiser;
		}

		delete background;
		delete system;
		delete hud;
		delete editor;
		/*
		if (loadThread != nullptr && loadThread->joinable()) {
			loadThread->join();
		}
		delete loadThread;
		*/

	}

	static void destroyInstance() {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	}

	/*
	* Called once, after the app is launched.
	*/
	void Initialise();

	void loadDefault();

	void control(float dt);

	void animate(float dt);

	void onContextResize(unsigned int windowWidth, unsigned int windowHeight);

	/*
	* Renders everything on screen.
	*/
	void draw(GPUProgram& gpuProgram);

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

	void toogleSimultaneBoost();

	/*
	* Switches between simultaneous hyperplane intersection and light cone intersection.
	*/
	void toggleIntersectionMode();

	void setIntersectionMode(RelTypes::IntersectionMode mode);

	void toggleViewMode();

	void toggleShading();
	
	void toggleHUD();

	void toggleDrawPath();

	void toggleDisplayIntersectable();

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

	void releaseGrab();

	/*
	* Mouse moved without regards to pressed button.
	* Receives the current mouse position on camera plane.
	*/
	void mouseMoved(float cX, float cY);


	void mouseScrolled(float delta);

	/*
	* When the mouse is moved with pressed buttons.
	*/
	void mouseDragged(const float cX, const float cY, const float deltaCX, const float deltaCY, const RelTypes::MouseState& mouseState);

	Entity* getUnderCursor(float cX, float cY);

	// Editor:

	void toggleEditorMode();

	void deleteSelected();

	// Getters:--------------------------------------------------------------------

	std::vector<LightSource*>* getLights() {
		return &lights;
	}

	std::vector<LightSource*>* getDiagramLights() {
		return &diagramLights;
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

	std::vector<Observer*>* getObservers() {
		return &observers;
	}

	Observer* getActiveObserver() {
		return activeObserver;
	}

	RelTypes::Settings& getSettings() {
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
		if (selected != nullptr) {
			selected->deselect();
			selected = nullptr;
			return false;
		}
		return allowQuit;
	}

	//For testing:
	std::vector<WorldLine*>& getLines() {
		return linesToDisplay;
	}

	void pushBackControlEvent(IControlEvent* e) {
		controlEvents.push_back(e);
	}

	void setFinishedLoading(bool b) {
		finishedLoading = b;
	}

	vec4 getEditedLocation(const float cX, const float cY);

	void setCameraDirectionMode(RelTypes::DirectionMode mode);

	bool isEntryMode();

	void beginCreationSequence();
	void finishCreationSequence();
	void cancelCreationSequence();

	CreationSequence getCreationSequence() {
		return creationSequence;
	}

	Initialiser* getInitialiser() {
		return initialiser;
	}

	void recreateObjectPointWorldLines() {
		for (auto obj : objects) {
			obj->recreatePointWorldLines(settings);
		}
	}

};


void saveHandler(const char* str);

void loadHandler(const char* str);
