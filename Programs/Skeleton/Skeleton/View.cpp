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
	for each (Caption * cap in *(scene->getCaptions()))				// Captions
	{
		cap->Draw(gpuProgram, *activeCamera);
	}
	delete intersectable;
}

void DiagramView::Draw(GPUProgram& gpuProgram) {
	Scene* scene = reinterpret_cast<Scene*>(owner);
	for each (LightSource * lt in *(scene->getDiagramLights()))
	{
		lt->loadOnGPU(gpuProgram);									// Load lights
	}

	//Actual drawing:
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
		cap->DrawDiagram(gpuProgram, *activeCamera);
	}
	scene->getBackground()->DrawDiagram(gpuProgram, *(scene->getActiveCamera()));		// Background
	delete intersectable;
}
