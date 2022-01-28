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
	Scene::getInstance()->zoomCamera(1 - 0.01f * dt * Scene::getInstance()->getCameraVelocity());
}

void ZoomOutEvent::performAction(const float dt)
{
	Scene::getInstance()->zoomCamera(1 + 0.01f * dt * Scene::getInstance()->getCameraVelocity());
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
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::Xlocked);
	if (Scene::getInstance()->getSettings().viewMode == RelTypes::ViewMode::diagram) {
		Scene::getInstance()->getActiveCamera()->translateTo(vec3(-25, 0, 0));
		Scene::getInstance()->getActiveCamera()->setLookat(vec3(0, 0, 0));
	}
}

void SelectYCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::Ylocked);
	if (Scene::getInstance()->getSettings().viewMode == RelTypes::ViewMode::diagram) {
		Scene::getInstance()->getActiveCamera()->translateTo(vec3(0, -25, 0));
		Scene::getInstance()->getActiveCamera()->setLookat(vec3(0, 0, 0));
	}
}

void SelectZCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::Zlocked);
	if (Scene::getInstance()->getSettings().viewMode == RelTypes::ViewMode::diagram) {
		Scene::getInstance()->getActiveCamera()->translateTo(vec3(0, 0, -25));
		Scene::getInstance()->getActiveCamera()->setLookat(vec3(0, 0, 0));
	}
}

void SelectFreeCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::free);
}

void SelectMinusXCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::minusXlocked);
	if (Scene::getInstance()->getSettings().viewMode == RelTypes::ViewMode::diagram) {
		Scene::getInstance()->getActiveCamera()->translateTo(vec3(25, 0, 0));
		Scene::getInstance()->getActiveCamera()->setLookat(vec3(0, 0, 0));
	}
}

void SelectMinusYCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::minusYlocked);
	if (Scene::getInstance()->getSettings().viewMode == RelTypes::ViewMode::diagram) {
		Scene::getInstance()->getActiveCamera()->translateTo(vec3(0, 25, 0));
		Scene::getInstance()->getActiveCamera()->setLookat(vec3(0, 0, 0));
	}
}

void SelectMinusZCameraDirectionEvent::performAction(const float dt)
{
	Scene::getInstance()->getActiveCamera()->selectDirectionMode(RelTypes::DirectionMode::minusZlocked);
	if (Scene::getInstance()->getSettings().viewMode == RelTypes::ViewMode::diagram) {
		Scene::getInstance()->getActiveCamera()->translateTo(vec3(0, 0, 25));
		Scene::getInstance()->getActiveCamera()->setLookat(vec3(0, 0, 0));
	}
}
