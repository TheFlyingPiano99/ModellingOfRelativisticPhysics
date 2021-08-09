#pragma once

#include "Camera.h"
#include "Observer.h"
#include "Object.h"

#include <vector>
#include <iostream>

class Scene {
	float timeScale = 1; // Time scale of the simulation.
	float absoluteTimeSpent = 0.0f;

	Camera* camera;

	Observer* currentObserver = NULL;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;

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
	}

	void Create();

	void Control(float dt) {
		dt *= timeScale;
		absoluteTimeSpent += dt;
	}

	void Animate(float dt) {
		dt *= timeScale;
	}

	void Draw(GPUProgram& gpuProgram) {
		if (currentObserver != nullptr) {
			camera->update(
				currentObserver->getLocationAtAbsoluteTime(absoluteTimeSpent),
				currentObserver->getVelocityAtAbsoluteTime(absoluteTimeSpent)
			);
			camera->loadOnGPU(gpuProgram);
			for each (Object * obj in objects)
			{
				obj->Draw(gpuProgram, *camera);
			}
		}
		std::cout << absoluteTimeSpent << std::endl;
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


};