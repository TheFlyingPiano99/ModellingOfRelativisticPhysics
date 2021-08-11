#pragma once

#include <vector>
#include <iostream>

#include "Camera.h"
#include "Observer.h"
#include "Object.h"
#include "Background.h"
#include "TextOperations.h"
#include "MenuSystem.h"

class Scene {
	float timeScale = 10; // Time scale of the simulation. (1 reallife millisec = to "timeScale" * 1[m])
	float absoluteTimeSpent = 0.0f;

	Camera* camera = NULL;

	Observer* currentObserver = NULL;
	std::vector<Observer*> observers;
	std::vector<Object*> objects;
	std::vector<Caption*> captions;

	Background* background;

	MenuSystem menu;
	bool running = true;
	void* defaultFont = GLUT_BITMAP_HELVETICA_18;

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
		for each (Caption * cap in captions)
		{
			delete cap;
		}

		delete background;
	}

	void Create();

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
			camera->update(
				currentObserver->getLocationAtAbsoluteTime(absoluteTimeSpent),
				currentObserver->getVelocityAtAbsoluteTime(absoluteTimeSpent)
			);
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

	void Draw(GPUProgram& gpuProgram) {
		if (currentObserver != nullptr) {
			//Prefase:
			gpuProgram.setUniform(RelPhysics::speedOfLight, "speedOfLight");
			camera->loadOnGPU(gpuProgram);

			//Actual drawing:
			background->Draw(gpuProgram, *camera);		// Background
			for each (Object * obj in objects)			// Objects
			{
				obj->Draw(gpuProgram, *camera);
			}
		}
		for each (Caption* cap in captions)
		{
			cap->Draw();
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

	void togglePause() {
		running = !running;
		captions.push_back(new Caption(vec2(200.0f, 200.0f), defaultFont, vec3(0.0f, 1.0f, 0.5f), "Paused"));
	}

};