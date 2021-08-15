#include "Object.h"

/*
* Factory for Earth-like object.
*/

Object* Object::createEarth(WorldLine* wrdln) {
	AdvancedTexture* texture = new AdvancedTexture("../../../Resources/lowres/earth_daymap.bmp", "../../../Resources/lowres/earth_normal_map.bmp", "");
	if (texture->getTextureId() == 0) {
		texture = NULL;
	}
	Material* diagramM = new Material(vec3(3, 1.5, 1), vec3(0, 0.3, 0.6), vec3(5, 6, 20), 50);
	diagramM->setGlow(true);
	ParamSurface* pSurface = new SphereSurface(0.5f);
	pSurface->GenSurface(100, 100);
	Object* obj = new Object(
		vec3(1.0f, 1.0f, 1.0f),
		0.0f,
		0.0f,
		vec3(2.5f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		wrdln,
		pSurface,
		new Material(vec3(3, 1.5, 1), vec3(5, 5, 5), vec3(5, 6, 20), 50),		// RealTime3D material
		diagramM,		// Diagram material
		texture,
		"Earth",
		"Inhabited planet"
	);
	return obj;
}

void Object::Animate(float dt) {
	//Todo
}

void Object::Draw(GPUProgram& gpuProgram, Camera& camera) {
	//geometry->updateBeforeDraw(camera.getVelocityFV(), camera.getLocationFV(), *worldLine);

	worldLine->loadOnGPU(gpuProgram);
	material->loadOnGPU(gpuProgram);
	if (texture != nullptr) {
		texture->loadOnGPU(gpuProgram);
	}
	gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
	//gpuProgram.setUniform(M(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(texture == nullptr, "noTexture");

	geometry->Draw();
}

void Object::DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
	diagramMaterial->loadOnGPU(gpuProgram);
	gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
	gpuProgram.setUniform(UnitMatrix(), "M");
	gpuProgram.setUniform(UnitMatrix(), "invM");
	gpuProgram.setUniform(true, "noTexture");

	worldLine->Draw();

}
