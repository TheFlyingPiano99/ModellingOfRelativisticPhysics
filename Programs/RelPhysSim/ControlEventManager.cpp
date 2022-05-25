#include "ControlEventManager.h"
#include "ControlEvent.h"

ControlEventManager* ControlEventManager::instance = nullptr;

ControlEventManager& ControlEventManager::getInstance() {
	if (instance == nullptr) {
		instance = new ControlEventManager();
	}
	return *instance;
}

void ControlEventManager::destroyInstance() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

void ControlEventManager::addControlEvent(IControlEvent* controlEvent)
{
	controlEvents.push_back(controlEvent);
}

std::vector<IControlEvent*>& ControlEventManager::getControlEvents()
{
	return controlEvents;
}

void ControlEventManager::handlePressedEvents(Scene& scene, unsigned char key)
{
	for (IControlEvent* event : controlEvents) {
		if (event->isPressed(key)) {
			scene.pushBackControlEvent(event);
		}
	}
}

void ControlEventManager::buildDefaultControlScheme()
{
	controlEvents.push_back(new ToggleObserverEvent());
	controlEvents.push_back(new TogglePauseEvent());
	controlEvents.push_back(new ZoomInEvent());
	controlEvents.push_back(new ZoomOutEvent());
	controlEvents.push_back(new ToggleDopplerEffectEvent());
	controlEvents.push_back(new RewindTimeEvent());
	controlEvents.push_back(new WindTimeEvent());
	controlEvents.push_back(new ToggleIntersectionModeEvent());
	controlEvents.push_back(new ToggleTransformToProperFrameEvent());
	controlEvents.push_back(new ToggleLorentzEvent());
	controlEvents.push_back(new ToggleViewModeEvent());
	controlEvents.push_back(new ToggleShadingEvent());
	controlEvents.push_back(new ToggleSelectionEvent());
	controlEvents.push_back(new MoveCameraForwardEvent());
	controlEvents.push_back(new MoveCameraBackwardEvent());
	controlEvents.push_back(new MoveCameraLeftEvent());
	controlEvents.push_back(new MoveCameraRightEvent());
	controlEvents.push_back(new MoveCameraUpEvent());
	controlEvents.push_back(new MoveCameraDownEvent());
	controlEvents.push_back(new SaveEvent());
	controlEvents.push_back(new LoadEvent());
	controlEvents.push_back(new ToggleEditorEvent());
	controlEvents.push_back(new ClearSceneEvent());
	controlEvents.push_back(new DeleteSelectedEvent());
	controlEvents.push_back(new ToggleSimultaneBoostEvent());
	controlEvents.push_back(new ToggleHUDEvent());
	controlEvents.push_back(new ToggleDisplayIntersectableEvent());
	controlEvents.push_back(new ToggleDrawPathEvent());
	/*
	controlEvents.push_back(new SelectFreeCameraDirectionEvent());
	controlEvents.push_back(new SelectXCameraDirectionEvent());
	controlEvents.push_back(new SelectYCameraDirectionEvent());
	controlEvents.push_back(new SelectZCameraDirectionEvent());
	controlEvents.push_back(new SelectMinusXCameraDirectionEvent());
	controlEvents.push_back(new SelectMinusYCameraDirectionEvent());
	controlEvents.push_back(new SelectMinusZCameraDirectionEvent());
	*/
}
