#include "ControlEvent.h"
#include "Scene.h"

void TogglePauseEvent::performAction( float dt) 
{
	Scene::getInstance()->togglePause();
}

void ToggleObserverEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleActiveObserver();
}

void ToggleViewModeEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleViewMode();
}

void ToggleIntersectionModeEvent::performAction( float dt)  {
	Scene::getInstance()->toggleIntersectionMode();
}

void ToggleSelectionEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleSelected();
}

void ToggleDopplerEffectEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleDoppler();
}

void ToggleShadingEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleShading();
}

void MoveCameraForwardEvent::performAction( float dt) 
{
	Scene::getInstance()->moveCamera(vec3(1, 0, 0) * dt);
}

void MoveCameraBackwardEvent::performAction( float dt) 
{
	Scene::getInstance()->moveCamera(vec3(-1, 0, 0) * dt * Scene::getInstance()->getCameraVelocity());
}

void MoveCameraUpEvent::performAction( float dt) 
{
	Scene::getInstance()->moveCamera(vec3(0, 0, 1) * dt * Scene::getInstance()->getCameraVelocity());
}

void MoveCameraLeftEvent::performAction( float dt)  
{
	Scene::getInstance()->moveCamera(vec3(0, -1, 0) * dt * Scene::getInstance()->getCameraVelocity());
}

void MoveCameraRightEvent::performAction( float dt) 
{
	Scene::getInstance()->moveCamera(vec3(0, 1, 0) * dt * Scene::getInstance()->getCameraVelocity());
}

void ZoomInEvent::performAction( float dt) 
{
	Scene::getInstance()->zoomCamera(1 + 0.01f * dt * Scene::getInstance()->getCameraVelocity());
}

void ZoomOutEvent::performAction( float dt) 
{
	Scene::getInstance()->zoomCamera(1 - 0.01f * dt * Scene::getInstance()->getCameraVelocity());
}

void WindTimeEvent::performAction( float dt) 
{
	Scene::getInstance()->windTime(5);
}

void RewindTimeEvent::performAction( float dt) 
{
	Scene::getInstance()->windTime(-5);
}

void ToggleLorentzEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleLorentzTransformation();
}

void ToggleTransformToProperFrameEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleTransformToProperFrame();
}

void SaveEvent::performAction( float dt) 
{
	Scene::getInstance()->startSaveProcess();
}

void LoadEvent::performAction( float dt) 
{
	Scene::getInstance()->startLoadProcess();
}

void ToggleEditorEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleEditorMode();
}

void ClearSceneEvent::performAction( float dt) 
{
	Scene::getInstance()->clearScene();
	WorldLine* wrdln = new GeodeticLine(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "Obs1's world line");
	Observer* observer = new Observer(wrdln, "Obs1", "An observer");
	Scene::getInstance()->getObservers()->push_back(observer);
	Scene::getInstance()->toggleActiveObserver();
}

void DeleteSelectedEvent::performAction( float dt) 
{
	Scene::getInstance()->deleteSelected();
}

void ToggleSimultaneBoostEvent::performAction( float dt) 
{
	Scene::getInstance()->toogleSimultaneBoost();
}

void ToggleHUDEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleHUD();
}

void ToggleDisplayIntersectableEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleDisplayIntersectable();
}

void MoveCameraDownEvent::performAction( float dt) 
{
	Scene::getInstance()->moveCamera(vec3(0, 0, -1) * dt * Scene::getInstance()->getCameraVelocity());
}

void ToggleImGUIEvent::performAction( float dt) 
{

}

void SelectXCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::Xlocked);
}

void SelectYCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::Ylocked);
}

void SelectZCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::Zlocked);
}

void SelectFreeCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::free);
}

void SelectMinusXCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::minusXlocked);
}

void SelectMinusYCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::minusYlocked);
}

void SelectMinusZCameraDirectionEvent::performAction( float dt) 
{
	Scene::getInstance()->setCameraDirectionMode(RelTypes::DirectionMode::minusZlocked);
}

void ToggleDrawPathEvent::performAction( float dt) 
{
	Scene::getInstance()->toggleDrawPath();
}
