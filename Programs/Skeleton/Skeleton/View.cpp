#include "View.h"

#include "Scene.h"


void RealTime3DView::Draw(GPUProgram& gpuProgram) {
	glDisable(GL_CULL_FACE);
	Scene* scene = reinterpret_cast<Scene*>(owner);
	Camera* camera = scene->getActiveCamera();
	for each (LightSource * lt in *(scene->getLights()))
	{
		lt->loadOnGPU(gpuProgram);								// Load lights
	}

	scene->getBackground()->Draw(gpuProgram, *(camera));		// Background
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->Draw(gpuProgram, *(scene->getActiveCamera()));					// Objects
	}
	for each (Caption * cap in *(scene->getCaptions()))				// Captions
	{
		cap->Draw();
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
	for each (Object * obj in *(scene->getObjects()))
	{
		obj->DrawDiagram(gpuProgram, *activeCamera);			// Objects
	}
	for each (Observer * obs in *(scene->getObservers()))
	{
		obs->DrawDiagram(gpuProgram, *activeCamera);			// Observers
	}
	system->Draw(gpuProgram, *activeCamera);					// Coordinate system

	scene->getActiveObserver()->DrawExtras(gpuProgram, *activeCamera);

	for each (Caption * cap in *(scene->getCaptions()))						// Captions
	{
		cap->Draw();
	}
}
