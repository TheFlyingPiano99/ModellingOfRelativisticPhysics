#pragma once

#include <vector>

#include "IControlEvent.h"
#include "Scene.h"

class ControlEventManager
{
	static ControlEventManager* instance;

	std::vector<IControlEvent*> controlEvents;

	ControlEventManager() {};

public:
	~ControlEventManager() {
		for (auto event : controlEvents) {
			delete event;
		}
	};

	static ControlEventManager& getInstance();
	static void destroyInstance();

	void addControlEvent(IControlEvent* controlEvent);
	std::vector<IControlEvent*>& getControlEvents();
	void handlePressedEvents(Scene& scene, unsigned char);
	void buildDefaultControlScheme();
};

