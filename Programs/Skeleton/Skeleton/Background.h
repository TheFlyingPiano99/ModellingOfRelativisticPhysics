#pragma once

#include "framework.h"
#include "Geometry.h"
#include "Camera.h"
#include "Material.h"

class Background {
	SphereSurface* geometry;
	Material* material;
	Texture* texture;

public:
	Background() {
		geometry = new SphereSurface(100);
		geometry->GenSurface(100, 100);
		vec3 color = vec3(0.1,0.1, 0.1);
		material = new Material(color, color, color, 50);
		material->setGlow(true);
		texture = new Texture("../../../Resources/lowres/stars_milky_way.bmp");
	}

	~Background() {
		delete geometry;
		delete material;
	}



	void Draw(GPUProgram& gpuProgram, Camera& camera) {
		geometry->updateBeforeDraw(camera.getVelocityFV(), vec4(0,0,0,1),
			camera.getLocationFV(), vec4(0,0,0,0));
		material->loadOnGPU(gpuProgram);

		gpuProgram.setUniform(*texture, "textureUnit");
		gpuProgram.setUniform(TranslateMatrix(camera.getEye()) * camera.V() * camera.P(), "MVP");
		gpuProgram.setUniform(TranslateMatrix(-camera.getEye()), "invM");
		//gpuProgram.setUniform(M(), "M");

		glDisable(GL_CULL_FACE);
		geometry->Draw(gpuProgram, TranslateMatrix(camera.getEye()), camera.V(), camera.P());
		glEnable(GL_CULL_FACE);
	}



};
