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
	controlEvents.emplace(controlEvent->getKeyShortcut(), controlEvent);
}

std::map<char, IControlEvent*>& ControlEventManager::getControlEvents()
{
	return controlEvents;
}

void ControlEventManager::onPress(Scene& scene, char key)
{
	auto event = controlEvents.find(key);
	if (event != controlEvents.end()) {
		if (event->second->onPress()) {
			scene.pushBackControlEvent(event->second);
		}
	}
}

void ControlEventManager::onRelease(Scene& scene, char key)
{
	auto event = controlEvents.find(key);
	if (event != controlEvents.end()) {
		if (event->second->onRelease()) {
			scene.pushBackControlEvent(event->second);
		}
	}
}

void ControlEventManager::buildDefaultControlScheme()
{
	addControlEvent(new ToggleObserverEvent());
	addControlEvent(new TogglePauseEvent());
	addControlEvent(new ZoomInEvent());
	addControlEvent(new ZoomOutEvent());
	addControlEvent(new ToggleDopplerEffectEvent());
	addControlEvent(new RewindTimeEvent());
	addControlEvent(new WindTimeEvent());
	addControlEvent(new ToggleIntersectionModeEvent());
	addControlEvent(new ToggleTransformToProperFrameEvent());
	addControlEvent(new ToggleLorentzEvent());
	addControlEvent(new ToggleViewModeEvent());
	addControlEvent(new ToggleShadingEvent());
	addControlEvent(new ToggleSelectionEvent());
	addControlEvent(new MoveCameraForwardEvent());
	addControlEvent(new MoveCameraBackwardEvent());
	addControlEvent(new MoveCameraLeftEvent());
	addControlEvent(new MoveCameraRightEvent());
	addControlEvent(new MoveCameraUpEvent());
	addControlEvent(new MoveCameraDownEvent());
	addControlEvent(new SaveEvent());
	addControlEvent(new LoadEvent());
	addControlEvent(new ToggleEditorEvent());
	addControlEvent(new ClearSceneEvent());
	addControlEvent(new DeleteSelectedEvent());
	addControlEvent(new ToggleSimultaneBoostEvent());
	addControlEvent(new ToggleHUDEvent());
	addControlEvent(new ToggleDisplayIntersectableEvent());
	addControlEvent(new ToggleDrawPathEvent());
	/*
	addControlEvent(new SelectFreeCameraDirectionEvent());
	addControlEvent(new SelectXCameraDirectionEvent());
	addControlEvent(new SelectYCameraDirectionEvent());
	addControlEvent(new SelectZCameraDirectionEvent());
	addControlEvent(new SelectMinusXCameraDirectionEvent());
	addControlEvent(new SelectMinusYCameraDirectionEvent());
	addControlEvent(new SelectMinusZCameraDirectionEvent());
	*/
}
