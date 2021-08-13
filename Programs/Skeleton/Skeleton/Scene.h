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

enum IntersectionMode {
	lightCone,
	hyperplane
};

enum ViewMode {
	realTime3D,
	diagram
};

enum DopplerMode {
	full,
	mild,
	off
};

class Scene {
	float timeScale = 2; // Time scale of the simulation. (1 reallife millisec = to "timeScale" * 1[m])
	float absoluteTimeSpent = 0.0f;

	View* view;

	Camera* realTime3DCamera = NULL;
	Camera* diagramCamera = NULL;
	Camera* activeCamera = NULL;
	IntersectionMode intersectionMode = lightCone;
	DopplerMode dopplerMode = full;
	ViewMode viewMode = realTime3D;

	Observer* currentObserver = NULL;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	std::vector<LightSource*> diagramLights;
	std::vector<Caption*> captions;

	Background* background;

	MenuSystem menu;
	bool running = false;
	bool doLorentz = true;
	void* defaultFont = GLUT_BITMAP_HELVETICA_18;


public:

	Scene() {
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

		delete background;
	}

	/*
	* Called once, after the app is launched.
	*/
	void Initialise();

	void Control(float dt) {
		if (running) {
			dt *= timeScale;
			absoluteTimeSpent += dt;
			//Todo
		}
	}

	void Animate(float dt) {
		if (running) {
			dt *= timeScale;
			if (currentObserver != nullptr) {
				realTime3DCamera->syncToObserver(
					currentObserver->getLocationAtAbsoluteTime(absoluteTimeSpent),
					currentObserver->getVelocityAtAbsoluteTime(absoluteTimeSpent),
					currentObserver->getLocationAtAbsoluteTime(0.0f));
			}
			for each (Object * obj in objects)
			{
				obj->Animate(dt, absoluteTimeSpent);
			}
			for each (Caption * cap in captions)
			{
				cap->Animate();
			}

		}
	}

	void Draw(GPUProgram& gpuProgram);

	void toggleCurrentObserver();

	// Camera controls:
	void moveCamera(float cx, float cy);

	void zoomCamera(float delta) {
		activeCamera->zoom(delta);
	}

	// Symulation controls:
	void toggleDoppler() {
		dopplerMode = (DopplerMode)((3 > dopplerMode + 1) ? (dopplerMode + 1) : 0);
	}

	void toggleLorentzTransformation() {
		doLorentz = !doLorentz;
	}

	void toggleIntersectionType() {
		intersectionMode = (IntersectionMode)((2 > intersectionMode + 1) ? (intersectionMode + 1) : 0);
	}

	void toggleViewMode();


	//Time manipulation:

	void togglePause() {
		running = !running;
		captions.push_back(new Caption(vec2(200.0f, 200.0f), defaultFont, vec3(0.0f, 1.0f, 0.5f), "Paused"));
	}

	void setTime(float t) {
		absoluteTimeSpent = t;
		bool prevState = running;
		running = true;
		Animate(0.0f);
		running = prevState;
	}

	void reset() {
		setTime(0.0f);
		bool prevState = running;
		running = true;
		Animate(0.0f);
		running = prevState;
	}

	void windTime(float deltaT) {
		absoluteTimeSpent += deltaT;
		bool prevState = running;
		running = true;
		Animate(0.0f);
		running = prevState;
	}

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

};