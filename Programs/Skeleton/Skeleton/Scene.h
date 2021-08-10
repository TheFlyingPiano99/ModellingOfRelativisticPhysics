#pragma once

#include <vector>
#include <iostream>

#include "Camera.h"
#include "Observer.h"
#include "Object.h"
#include "Background.h"

class Scene {
	float timeScale = 10; // Time scale of the simulation. (1 reallife millisec = to "timeScale" * 1[m])
	float absoluteTimeSpent = 0.0f;

	Camera* camera = NULL;

	Observer* currentObserver = NULL;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;

	Background* background;

public:

	Scene() {
	}

	~Scene() {
		for each (Observer* obs in observers)
		{
			delete obs;
		}
		for each (Object * obj in objects)
		{
			delete obj;
		}

		delete background;
	}

	void Create();

	void Control(float dt) {
		dt *= timeScale;
		absoluteTimeSpent += dt;
	}

	void Animate(float dt) {
		dt *= timeScale;
		camera->update(
			currentObserver->getLocationAtAbsoluteTime(absoluteTimeSpent),
			currentObserver->getVelocityAtAbsoluteTime(absoluteTimeSpent)
		);
		for each (Object* obj in objects)
		{
			obj->Animate(dt, absoluteTimeSpent);
		}
	}

	void Draw(GPUProgram& gpuProgram) {
		if (currentObserver != nullptr) {
			//Prefase:
			camera->loadOnGPU(gpuProgram);

			//Actual drawing:
			background->Draw(gpuProgram, *camera);		// Background
			for each (Object * obj in objects)			// Objects
			{
				obj->Draw(gpuProgram, *camera);
			}
		}
	}

	void toggleCurrentObserver() {
		static int currentIdx = -1;
		if (!observers.empty()) {
			currentIdx = (observers.size() > currentIdx + 1)? currentIdx + 1 : 0;
			currentObserver = observers.at(currentIdx);
			camera->update(currentObserver->getLocationAtAbsoluteTime(absoluteTimeSpent),
				currentObserver->getVelocityAtAbsoluteTime(absoluteTimeSpent));
		}
	}

	void moveCamera(float cx, float cy) {
		static float camSpeed = 0.01f;
		camera->rotate(cx, -cy);
	}

};