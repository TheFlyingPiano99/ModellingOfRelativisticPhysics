#include "Observer.h"

vec4 Observer::getLocationAtAbsoluteTime(float t)
{
    return worldLine->getLocationAtAbsoluteTime(t);
}

vec4 Observer::getVelocityAtAbsoluteTime(float t)
{
    return worldLine->getVelocityAtAbsoluteTime(t);
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
