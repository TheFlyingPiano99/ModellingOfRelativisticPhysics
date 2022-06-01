#pragma once

#include <map>

#include "IControlEvent.h"
#include "Scene.h"

class ControlEventManager
{
	static ControlEventManager* instance;

	std::map<char, IControlEvent*> controlEvents;

	ControlEventManager() {};

public:
	~ControlEventManager() {
		for (auto event : controlEvents) {
			delete event.second;
		}
	};

	static ControlEventManager& getInstance();
	static void destroyInstance();

	void addControlEvent(IControlEvent* controlEvent);
	std::map<char, IControlEvent*>& getControlEvents();
	void onPress(Scene& scene, char key);
	void onRelease(Scene& scene, char key);
	void buildDefaultControlScheme();
};

