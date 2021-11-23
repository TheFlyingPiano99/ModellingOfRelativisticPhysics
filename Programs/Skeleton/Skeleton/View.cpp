#include "View.h"

#include "Scene.h"


void RealTime3DView::Draw(GPUProgram& gpuProgram) {
	glDisable(GL_CULL_FACE);
	Scene* scene = reinterpret_cast<Scene*>(owner);
	Camera* activeCamera = scene->getActiveCamera();
	LightCone* lightCone = scene->getActiveObserver()->getLightCone(scene->getSettings());
	Hyperplane* hyperplane = scene->getActiveObserver()->getHyperplane(scene->getSettings());

	for each (LightSource * lt in *(scene->getLights()))
	{
		lt->loadOnGPU(gpuProgram);								// Load lights
	}

	gpuProgram.setUniform(false, "doLorentz");
	gpuProgram.setUniform(1, "intersectionMode");
	scene->getBackground()->Draw(gpuProgram, *(activeCamera));		// Background
	gpuProgram.setUniform(scene->getSettings().doLorentz.get(), "doLorentz");
	gpuProgram.setUniform(scene->getSettings().intersectionMode.get(), "intersectionMode");

	for each (Object * obj in *(scene->getObjects()))
	{
		Settings settings = scene->getSettings();
		obj->Draw(gpuProgram, *(scene->getActiveCamera()), *lightCone, *hyperplane, scene->getActiveObserver()->getProperties(settings), settings);	// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->Draw(gpuProgram, *activeCamera);			// Observers
	}
	scene->getCoordinateSystem()->Draw(gpuProgram, *activeCamera);					// Coordinate system
	
	scene->getHUD()->Draw(gpuProgram, *activeCamera);				// HUD

	delete lightCone;
	delete hyperplane;
}

void RealTime3DView::transitionFrom()
{
}

void RealTime3DView::transitionTo()
{
}

void DiagramView::Draw(GPUProgram& gpuProgram) {
	Scene* scene = reinterpret_cast<Scene*>(owner);
	for each (LightSource * lt in *(scene->getDiagramLights()))
	{
		lt->loadOnGPU(gpuProgram);									// Load lights
	}

	//Actual drawing:
	Camera* activeCamera = scene->getActiveCamera();
	LightCone* lightCone = scene->getActiveObserver()->getLightCone(scene->getSettings());
	Hyperplane* hyperplane = scene->getActiveObserver()->getHyperplane(scene->getSettings());
	scene->getBackground()->DrawDiagram(gpuProgram, *(scene->getActiveCamera()));		// Background
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->DrawDiagram(gpuProgram, *activeCamera, *lightCone, *hyperplane,
			scene->getActiveObserver()->getProperties(scene->getSettings()),
			scene->getSettings());			// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->DrawDiagram(gpuProgram, *activeCamera);			// Observers
	}
	((Scene*)owner)->getCoordinateSystem()->DrawDiagram(gpuProgram, *activeCamera);					// Coordinate system

	scene->getActiveObserver()->DrawExtras(gpuProgram, *activeCamera, scene->getActiveObserver()->getProperties(scene->getSettings()), scene->getSettings());

	scene->getHUD()->DrawDiagram(gpuProgram, *activeCamera);			// HUD
	delete lightCone;
	delete hyperplane;
}

void DiagramView::transitionFrom()
{
}

void DiagramView::transitionTo()
{
}
