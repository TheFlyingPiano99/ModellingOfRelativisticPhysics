#include "Background.h"

void Background::Draw(GPUProgram& gpuProgram, Camera& camera) {
	WorldLine* line = new GeodeticLine(
		vec4(camera.getLocationFV().x, camera.getLocationFV().y, camera.getLocationFV().z, 0),
		vec4(0, 0, 0, 1),
		"Background world line", "");
	line->loadOnGPU(gpuProgram);
	delete line;
	material->loadOnGPU(gpuProgram);

	gpuProgram.setUniform(*texture, "textureUnit");
	gpuProgram.setUniform(camera.getViewMatrix() * camera.getPerspectiveProjectionMatrix(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(texture == nullptr, "noTexture");
	gpuProgram.setUniform(false, "outline");

	glDisable(GL_CULL_FACE);
	geometry->Draw();
	glEnable(GL_CULL_FACE);
	glClearDepth(1);
}

void Background::DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
	material->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(*texture, "textureUnit");
	gpuProgram.setUniform(camera.getViewMatrix() * camera.getPerspectiveProjectionMatrix(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(texture == nullptr, "noTexture");
	gpuProgram.setUniform(false, "outline");
	gpuProgram.setUniform(true, "directRenderMode");

	glDisable(GL_CULL_FACE);
	geometry->Draw();
	glEnable(GL_CULL_FACE);
	glClearDepth(1);
}
