#include "Background.h"

void Background::draw(const Camera& camera) {
	program->Use();
	texture->Bind();
	program->setUniform(camera.getViewMatrix() * camera.getPerspectiveProjectionMatrix(), "camera.viewProjMatrix");

	glDisable(GL_CULL_FACE);
	geometry->draw();
	glEnable(GL_CULL_FACE);
	glClearDepth(1);
}