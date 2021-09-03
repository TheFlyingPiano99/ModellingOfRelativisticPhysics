#include "View.h"

#include "Scene.h"


void RealTime3DView::Draw(GPUProgram& gpuProgram) {
	glDisable(GL_CULL_FACE);
	Scene* scene = reinterpret_cast<Scene*>(owner);
	Camera* activeCamera = scene->getActiveCamera();
	Intersectable* intersectable;
	if (scene->getSettings().intersectionMode == IntersectionMode::lightCone) {
		intersectable = scene->getActiveObserver()->getLightCone();
	}
	else if (scene->getSettings().intersectionMode == IntersectionMode::hyperplane) {
		intersectable = scene->getActiveObserver()->getHyperplane();
	}
	for each (LightSource * lt in *(scene->getLights()))
	{
		lt->loadOnGPU(gpuProgram);								// Load lights
	}

	gpuProgram.setUniform(false, "doLorentz");
	gpuProgram.setUniform(1, "intersectionMode");
	scene->getBackground()->Draw(gpuProgram, *(activeCamera));		// Background
	gpuProgram.setUniform(scene->getSettings().doLorentz, "doLorentz");
	gpuProgram.setUniform(scene->getSettings().intersectionMode, "intersectionMode");

	for each (Object * obj in *(scene->getObjects()))
	{
		obj->Draw(gpuProgram, *(scene->getActiveCamera()), *intersectable, scene->getSettings().doLorentz);					// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->Draw(gpuProgram, *activeCamera);			// Observers
	}
	scene->getCoordinateSystem()->Draw(gpuProgram, *activeCamera);					// Coordinate system
	
	scene->getHUD()->Draw(gpuProgram, *activeCamera);				// HUD

	delete intersectable;
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
	gpuProgram.setUniform(scene->getSettings().diagramX, "diagramX");
	gpuProgram.setUniform(scene->getSettings().diagramY, "diagramY");
	gpuProgram.setUniform(scene->getSettings().diagramZ, "diagramZ");

	//Actual drawing:
	Camera* activeCamera = scene->getActiveCamera();
	Intersectable* intersectable;
	scene->getBackground()->DrawDiagram(gpuProgram, *(scene->getActiveCamera()));		// Background
	if (scene->getSettings().intersectionMode == IntersectionMode::lightCone) {
		intersectable = scene->getActiveObserver()->getLightCone();
	}
	else if (scene->getSettings().intersectionMode == IntersectionMode::hyperplane) {
		intersectable = scene->getActiveObserver()->getHyperplane();
	}
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->DrawDiagram(gpuProgram, *activeCamera, *intersectable,
			scene->getActiveObserver()->getProperties(),
			scene->getSettings());			// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->DrawDiagram(gpuProgram, *activeCamera);			// Observers
	}
	((Scene*)owner)->getCoordinateSystem()->DrawDiagram(gpuProgram, *activeCamera);					// Coordinate system

	scene->getActiveObserver()->DrawExtras(gpuProgram, *activeCamera, scene->getActiveObserver()->getProperties(), scene->getSettings());

	scene->getHUD()->DrawDiagram(gpuProgram, *activeCamera);			// HUD
	delete intersectable;
}

void DiagramView::transitionFrom()
{
}

void DiagramView::transitionTo()
{
}
