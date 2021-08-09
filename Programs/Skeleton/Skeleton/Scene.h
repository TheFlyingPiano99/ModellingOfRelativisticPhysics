#pragma once

#include "Camera.h"
#include "Observer.h"

#include <vector>

class Scene {
	Camera* camera;
	std::vector<Observer*> observers;
	std::vector<Observer*> observers;


public:

	Scene() {

	}

	~Scene() {
		delete camera;
	}

	void Create();

};