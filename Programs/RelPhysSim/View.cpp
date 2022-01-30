#include "View.h"

#include "Scene.h"


void RealTime3DView::draw(GPUProgram& gpuProgram) {
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
	scene->getBackground()->draw(gpuProgram, *(activeCamera));		// Background
	gpuProgram.setUniform(scene->getSettings().doLorentz.get(), "doLorentz");
	gpuProgram.setUniform(scene->getSettings().intersectionMode.get(), "intersectionMode");

	for each (Object * obj in *(scene->getObjects()))
	{
		RelTypes::Settings settings = scene->getSettings();
		obj->draw(gpuProgram, *(scene->getActiveCamera()), *lightCone, *hyperplane, scene->getActiveObserver()->getProperties(settings), settings);	// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->draw(gpuProgram, *activeCamera);			// Observers
	}
	scene->getCoordinateSystem()->draw(gpuProgram, *activeCamera);					// Coordinate system
	
	scene->getHUD()->draw(gpuProgram, *activeCamera);				// HUD

	delete lightCone;
	delete hyperplane;
}

void RealTime3DView::transitionFrom()
{
}

void RealTime3DView::transitionTo()
{
}

void DiagramView::draw(GPUProgram& gpuProgram) {
	Scene* scene = reinterpret_cast<Scene*>(owner);
	for each (LightSource * lt in *(scene->getDiagramLights()))
	{
		lt->loadOnGPU(gpuProgram);									// Load lights
	}

	//Actual drawing:
	Camera* activeCamera = scene->getActiveCamera();
	LightCone* lightCone = scene->getActiveObserver()->getLightCone(scene->getSettings());
	Hyperplane* hyperplane = scene->getActiveObserver()->getHyperplane(scene->getSettings());
	scene->getBackground()->drawDiagram(gpuProgram, *(scene->getActiveCamera()));		// Background
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->drawDiagram(gpuProgram, *activeCamera, *lightCone, *hyperplane,
			scene->getActiveObserver()->getProperties(scene->getSettings()),
			scene->getSettings());			// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->drawDiagram(gpuProgram, *activeCamera);			// Observers
	}
	((Scene*)owner)->getCoordinateSystem()->drawDiagram(gpuProgram, *activeCamera);					// Coordinate system

	scene->getActiveObserver()->drawExtras(gpuProgram, *activeCamera, scene->getActiveObserver()->getProperties(scene->getSettings()), scene->getSettings());

	scene->getHUD()->draw(gpuProgram, *activeCamera);			// HUD
	delete lightCone;
	delete hyperplane;
}

void DiagramView::transitionFrom()
{
}

void DiagramView::transitionTo()
{
}
