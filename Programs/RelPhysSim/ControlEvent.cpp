#include "ControlEvent.h"
#include "Scene.h"

void TogglePauseEvent::performAction(const float dt)
{
	Scene::getInstance()->togglePause();
}

void ToggleObserverEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleActiveObserver();
}

void ToggleViewModeEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleViewMode();
}

void ToggleIntersectionModeEvent::performAction(const float dt) {
	Scene::getInstance()->toggleIntersectionMode();
}

void ToggleSelectionEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleSelected();
}

void ToggleDopplerEffectEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleDoppler();
}

void ToggleShadingEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleShading();
}

void MoveCameraForwardEvent::performAction(const float dt)
{
	Scene::getInstance()->moveCamera(vec3(1, 0, 0) * dt);
}

void MoveCameraBackwardEvent::performAction(const float dt)
{
	Scene::getInstance()->moveCamera(vec3(-1, 0, 0) * dt * Scene::getInstance()->getCameraVelocity());
}

void MoveCameraUpEvent::performAction(const float dt)
{
	Scene::getInstance()->moveCamera(vec3(0, 0, 1) * dt * Scene::getInstance()->getCameraVelocity());
}

void MoveCameraLeftEvent::performAction(const float dt) 
{
	Scene::getInstance()->moveCamera(vec3(0, -1, 0) * dt * Scene::getInstance()->getCameraVelocity());
}

void MoveCameraRightEvent::performAction(const float dt)
{
	Scene::getInstance()->moveCamera(vec3(0, 1, 0) * dt * Scene::getInstance()->getCameraVelocity());
}

void ZoomInEvent::performAction(const float dt)
{
	Scene::getInstance()->zoomCamera(1 + 0.01f * dt * Scene::getInstance()->getCameraVelocity());
}

void ZoomOutEvent::performAction(const float dt)
{
	Scene::getInstance()->zoomCamera(1 - 0.01f * dt * Scene::getInstance()->getCameraVelocity());
}

void WindTimeEvent::performAction(const float dt)
{
	Scene::getInstance()->windTime(5);
}

void RewindTimeEvent::performAction(const float dt)
{
	Scene::getInstance()->windTime(-5);
}

void ToggleLorentzEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleLorentzTransformation();
}

void ToggleTransformToProperFrameEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleTransformToProperFrame();
}

void SaveEvent::performAction(const float dt)
{
	Scene::getInstance()->startSaveProcess();
}

void LoadEvent::performAction(const float dt)
{
	Scene::getInstance()->startLoadProcess();
}

void ToggleEditorEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleEditorMode();
}

void ClearSceneEvent::performAction(const float dt)
{
	Scene::getInstance()->clearScene();
	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	Scene::getInstance()->getObservers()->push_back(observer);
	Scene::getInstance()->toggleActiveObserver();
}

void DeleteSelectedEvent::performAction(const float dt)
{
	Scene::getInstance()->deleteSelected();
}

void ToggleSimultaneBoostEvent::performAction(const float dt)
{
	Scene::getInstance()->toogleSimultaneBoost();
}

void ToggleHUDEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleHUD();
}

void ToggleDisplayIntersectableEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleDisplayIntersectable();
}

void MoveCameraDownEvent::performAction(const float dt)
{
	Scene::getInstance()->moveCamera(vec3(0, 0, -1) * dt * Scene::getInstance()->getCameraVelocity());
}

void ToggleImGUIEvent::performAction(const float dt)
{

}

void SelectXCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::Xlocked);
}

void SelectYCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::Ylocked);
}

void SelectZCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::Zlocked);
}

void SelectFreeCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::free);
}

void SelectMinusXCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::minusXlocked);
}

void SelectMinusYCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::minusYlocked);
}

void SelectMinusZCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::minusZlocked);
}

void ToggleDrawPathEvent::performAction(const float dt)
{
	Scene::getInstance()->toggleDrawPath();
}
