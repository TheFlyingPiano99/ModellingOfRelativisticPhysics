#pragma once

#include <vector>
#include <iostream>

#include "Camera.h"
#include "Observer.h"
#include "Object.h"
#include "Background.h"
#include "TextOperations.h"
#include "MenuSystem.h"
#include "LightSource.h"
#include "View.h"
#include "EnumTypes.h"
#include <map>

class Scene {
	float timeScale = 2; // Time scale of the simulation. (1 reallife millisec = to "timeScale" * 1[m])
	View* view;
	Camera* realTime3DCamera = NULL;
	Camera* diagramCamera = NULL;
	Camera* activeCamera = NULL;
	IntersectionMode intersectionMode = lightCone;
	DopplerMode dopplerMode = full;
	ViewMode viewMode = realTime3D;

	Observer* activeObserver = NULL;
	Entity* selected = NULL;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	std::vector<LightSource*> diagramLights;
	std::vector<Caption*> captions;
	std::vector<WorldLine*> linesToDisplay;				// For testing only!!!

	Background* background;

	MenuSystem menu;
	bool running = false;
	bool doLorentz = true;
	bool doShading = true;
	void* defaultFont = GLUT_BITMAP_HELVETICA_18;


public:


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
		for each (Caption * cap in captions)
		{
			delete cap;
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

	// Camera controls:
	void moveCamera(float cx, float cy);

	void zoomCamera(float delta);

	// Symulation controls:
	void toggleDoppler();

	void toggleLorentzTransformation();

	/*
	* Switches between simultaneous hyperplane intersection and light cone intersection.
	*/
	void toggleIntersectionType();

	void toggleViewMode();

	void toggleShading();


	// Time manipulation:

	void togglePause();

	void setTime(float t);

	void reset();

	void windTime(float deltaT);

	// Selection:

	void toggleSelected();

	/*
	* Select object, where the mouse clicked
	* Receives X, and Y coordinates in camera space... [-1; 1]
	*/
	void selectByClick(float cX, float cY);

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

	std::vector<Caption*>* getCaptions() {
		return &captions;
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


	//For testing:
	std::vector<WorldLine*>& getLines() {
		return linesToDisplay;
	}

};