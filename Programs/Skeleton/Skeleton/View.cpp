#include "View.h"

#include "Scene.h"


void RealTime3DView::Draw(GPUProgram& gpuProgram) {
	glDisable(GL_CULL_FACE);
	Scene* scene = reinterpret_cast<Scene*>(owner);
	Camera* activeCamera = scene->getActiveCamera();
	Intersectable* intersectable;
	if (scene->getIntersectionMode() == IntersectionMode::lightCone) {
		intersectable = scene->getActiveObserver()->getLightCone();
	}
	else if (scene->getIntersectionMode() == IntersectionMode::hyperplane) {
		intersectable = scene->getActiveObserver()->getHyperplane();
	}
	for each (LightSource * lt in *(scene->getLights()))
	{
		lt->loadOnGPU(gpuProgram);								// Load lights
	}

	gpuProgram.setUniform(false, "doLorentz");
	gpuProgram.setUniform(1, "intersectionMode");
	scene->getBackground()->Draw(gpuProgram, *(activeCamera));		// Background
	gpuProgram.setUniform(scene->getDoLorentz(), "doLorentz");
	gpuProgram.setUniform(scene->getIntersectionMode(), "intersectionMode");

	for each (Object * obj in *(scene->getObjects()))
	{
		obj->Draw(gpuProgram, *(scene->getActiveCamera()), *intersectable, scene->getDoLorentz());					// Objects
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
	gpuProgram.setUniform(scene->getDiagramX(), "diagramX");
	gpuProgram.setUniform(scene->getDiagramY(), "diagramY");
	gpuProgram.setUniform(scene->getDiagramZ(), "diagramZ");

	//Actual drawing:
	Camera* activeCamera = scene->getActiveCamera();
	Intersectable* intersectable;
	scene->getBackground()->DrawDiagram(gpuProgram, *(scene->getActiveCamera()));		// Background
	if (scene->getIntersectionMode() == IntersectionMode::lightCone) {
		intersectable = scene->getActiveObserver()->getLightCone();
	}
	else if (scene->getIntersectionMode() == IntersectionMode::hyperplane) {
		intersectable = scene->getActiveObserver()->getHyperplane();
	}
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->DrawDiagram(gpuProgram, *activeCamera, *intersectable,
			scene->getActiveObserver()->getStartPos(),
			scene->getActiveObserver()->getVelocity(),
			scene->getDiagramX(),
			scene->getDiagramY(),
			scene->getDiagramZ());			// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->DrawDiagram(gpuProgram, *activeCamera);			// Observers
	}
	((Scene*)owner)->getCoordinateSystem()->DrawDiagram(gpuProgram, *activeCamera);					// Coordinate system

	scene->getActiveObserver()->DrawExtras(gpuProgram, *activeCamera, scene->getIntersectionMode());

	scene->getHUD()->DrawDiagram(gpuProgram, *activeCamera);			// HUD
	delete intersectable;
}

void DiagramView::transitionFrom()
{
}

void DiagramView::transitionTo()
{
}
