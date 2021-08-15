#include "Observer.h"
#include "Geometry.h"
#include "DiagramAssets.h"

vec4 Observer::getLocation()
{
    return worldLine->getLocationAtProperTime(currentProperTime);
}

vec4 Observer::getVelocity()
{
    return worldLine->getVelocityAtProperTime(currentProperTime);
}

vec4 Observer::getStartPos()
{
	return worldLine->getLocationAtAbsoluteTime(0.0f);
}

void Observer::DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
	diagramMaterial->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");

	worldLine->Draw();
}

void Observer::DrawHyperplane(GPUProgram& gpuProgram, Camera& camera)
{
	diagramMaterial->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(getLocation().x, getLocation().y, getLocation().w)), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(0.4f, "transparency");
	PlaneSurface* plane = new PlaneSurface(normalize(vec3(-getVelocity().x, -getVelocity().y, getVelocity().w)), 100, 100);
	plane->GenSurface(20, 20);
	glDisable(GL_CULL_FACE);
	plane->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawLightCone(GPUProgram& gpuProgram, Camera& camera)
{
	diagramMaterial->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(getLocation().x, getLocation().y, getLocation().w)), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	gpuProgram.setUniform(0.4f, "transparency");
	glDisable(GL_CULL_FACE);
	DiagramAssets::getLightConeGeomtry()->Draw();
	glEnable(GL_CULL_FACE);
}

void Observer::DrawNode(GPUProgram& gpuProgram, Camera& camera)
{
	diagramMaterial->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)) * camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(TranslateMatrix(vec3(getLocation().x, getLocation().y, getLocation().w)), "M");
	gpuProgram.setUniform(TranslateMatrix(-vec3(getLocation().x, getLocation().y, getLocation().w)), "invM");
	gpuProgram.setUniform(true, "glow");
	gpuProgram.setUniform(true, "noTexture");
	DiagramAssets::getObserverNodeGeometry()->Draw();
}

void Observer::DrawExtras(GPUProgram& gpuProgram, Camera& camera)
{
	DrawLightCone(gpuProgram, camera);
	DrawHyperplane(gpuProgram, camera);
	DrawNode(gpuProgram, camera);
}

void Observer::setTimeAtAbsoluteTime(float t)
{
	currentProperTime = worldLine->getProperTimeAtAbsoluteTime(t);
}

void Observer::increaseTimeByDelta(float deltaTau)
{
	currentProperTime += deltaTau;
}

void Observer::syncCamera(Camera* camera)
{
	camera->syncToObserver(
		getLocation(),
		getVelocity(),
		getStartPos());
}

void Observer::loadOnGPU(GPUProgram& gpuProgram)
{
	gpuProgram.setUniform(getVelocity(), "observersVelocity");
	gpuProgram.setUniform(getLocation(), "observersLocation");
	gpuProgram.setUniform(getStartPos(), "observersStartPos");
}
