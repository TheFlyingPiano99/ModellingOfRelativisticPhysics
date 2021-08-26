#include "View.h"

#include "Scene.h"


void RealTime3DView::Draw(GPUProgram& gpuProgram) {
	glDisable(GL_CULL_FACE);
	Scene* scene = reinterpret_cast<Scene*>(owner);
	Camera* activeCamera = scene->getActiveCamera();
	for each (LightSource * lt in *(scene->getLights()))
	{
		lt->loadOnGPU(gpuProgram);								// Load lights
	}

	scene->getBackground()->Draw(gpuProgram, *(activeCamera));		// Background
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->Draw(gpuProgram, *(scene->getActiveCamera()));					// Objects
	}
	for each (Caption * cap in *(scene->getCaptions()))				// Captions
	{
		cap->Draw(gpuProgram, *activeCamera);
	}

	/*
	//Testing:
	for each (WorldLine* line in scene->getLines()) {
		line->Draw();
	}
	*/
}

void DiagramView::Draw(GPUProgram& gpuProgram) {
	Scene* scene = reinterpret_cast<Scene*>(owner);
	for each (LightSource * lt in *(scene->getDiagramLights()))
	{
		lt->loadOnGPU(gpuProgram);									// Load lights
	}

	//Actual drawing:
	scene->getBackground()->DrawDiagram(gpuProgram, *(scene->getActiveCamera()));		// Background
	Camera* activeCamera = scene->getActiveCamera();
	Intersectable* intersectable;
	if (scene->getIntersectionMode() == IntersectionMode::lightCone) {
		intersectable = scene->getActiveObserver()->getLightCone();
	}
	else if (scene->getIntersectionMode() == IntersectionMode::hyperplane) {
		intersectable = scene->getActiveObserver()->getHyperplane();
	}
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->DrawDiagram(gpuProgram, *activeCamera, *intersectable);			// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->DrawDiagram(gpuProgram, *activeCamera);			// Observers
	}
	system->Draw(gpuProgram, *activeCamera);					// Coordinate system

	scene->getActiveObserver()->DrawExtras(gpuProgram, *activeCamera, scene->getIntersectionMode());

	for each (Caption * cap in *(scene->getCaptions()))						// Captions
	{
		cap->Draw(gpuProgram, *activeCamera);
	}
}
