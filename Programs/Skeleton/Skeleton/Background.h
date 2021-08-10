#pragma once

#include "framework.h"
#include "Geometry.h"
#include "Camera.h"
#include "Material.h"

class Background {
	Stars* stars;
	Material* material;

public:
	Background() {
		stars = new Stars();
		stars->GenSurface(100, 100);
		vec3 color = vec3(1,1,1);
		material = new Material(color, color, color, 50);
		material->setGlow(true);
	}

	~Background() {
		delete stars;
		delete material;
	}



	void Draw(GPUProgram& gpuProgram, Camera& camera) {
		stars->updateBeforeDraw(camera.getVelocityFV(), vec4(0,0,0,1),
			camera.getLocationFV(), vec4(0,0,0,0));
		material->loadOnGPU(gpuProgram);
		
		gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
		//gpuProgram.setUniform(M(), "M");
		gpuProgram.setUniform(UnitMatrix(), "invM");

		stars->Draw(gpuProgram, UnitMatrix(), camera.V(), camera.P());
	}



};
