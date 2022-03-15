#include "Background.h"

void Background::draw(GPUProgram& gpuProgram, const Camera& camera) {
	vec3 eye = camera.getEye();
	WorldLine* line = new GeodeticLine(
		vec4(eye.x, eye.y, eye.z, 0),
		vec4(0, 0, 0, 1),
		"Background world line", "");
	line->loadOnGPU(gpuProgram);
	delete line;
	material->loadOnGPU(gpuProgram);

	gpuProgram.setUniform(*texture, "textureUnit");
	gpuProgram.setUniform(camera.getViewMatrix() * camera.getPerspectiveProjectionMatrix(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "getModellMatrix");
	gpuProgram.setUniform(UnitMatrix(), "getInverseModellMatrix");
	gpuProgram.setUniform(texture == nullptr, "noTexture");
	gpuProgram.setUniform(false, "outline");

	glDisable(GL_CULL_FACE);
	geometry->draw();
	glEnable(GL_CULL_FACE);
	glClearDepth(1);
}

void Background::drawDiagram(GPUProgram& gpuProgram, const Camera& camera) {
	material->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(*texture, "textureUnit");
	gpuProgram.setUniform(camera.getViewMatrix() * camera.getPerspectiveProjectionMatrix(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "getInverseModellMatrix");
	gpuProgram.setUniform(UnitMatrix(), "getModellMatrix");
	gpuProgram.setUniform(texture == nullptr, "noTexture");
	gpuProgram.setUniform(false, "outline");
	gpuProgram.setUniform(true, "directRenderMode");

	glDisable(GL_CULL_FACE);
	geometry->draw();
	glEnable(GL_CULL_FACE);
	glClearDepth(1);
}
