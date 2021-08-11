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
		geometry->GenSurface(50, 50);
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
		WorldLine* line = new GeodeticLine(
			vec4(camera.getLocationFV().x, camera.getLocationFV().y, camera.getLocationFV().z, 0),
			vec4(0, 0, 0, 1),
			"Background world line", "");
		geometry->updateBeforeDraw(
			camera.getVelocityFV(),
			camera.getLocationFV(),
			camera.getHyperplane(), *line);
		delete line;
		material->loadOnGPU(gpuProgram);

		gpuProgram.setUniform(*texture, "textureUnit");
		gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
		gpuProgram.setUniform(UnitMatrix(), "invM");
		//gpuProgram.setUniform(M(), "M");

		glDisable(GL_CULL_FACE);
		geometry->Draw(gpuProgram, UnitMatrix(), camera.V(), camera.P());
		glEnable(GL_CULL_FACE);
	}



};
