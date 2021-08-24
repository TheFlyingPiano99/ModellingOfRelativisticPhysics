#pragma once

#include "framework.h"
#include "Geometry.h"
#include "Camera.h"
#include "Material.h"
#include "Assets.h"

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
		texture = new Texture(Assets::getTexturePath().append("stars_milky_way.bmp"));
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
/*		geometry->updateBeforeDraw(
			camera.getVelocityFV(),
			camera.getLocationFV(),
			*line);*/
		line->loadOnGPU(gpuProgram);
		delete line;
		material->loadOnGPU(gpuProgram);

		gpuProgram.setUniform(*texture, "textureUnit");
		gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
		gpuProgram.setUniform(UnitMatrix(), "M");
		gpuProgram.setUniform(UnitMatrix(), "invM");
		gpuProgram.setUniform(texture == nullptr, "noTexture");
		gpuProgram.setUniform(false, "outline");

		glDisable(GL_CULL_FACE);
		geometry->Draw();
		glEnable(GL_CULL_FACE);
	}

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera) {
		material->loadOnGPU(gpuProgram);
		gpuProgram.setUniform(*texture, "textureUnit");
		gpuProgram.setUniform(camera.Translate() * camera.V() * camera.P(), "MVP");
		gpuProgram.setUniform(UnitMatrix(), "invM");
		gpuProgram.setUniform(UnitMatrix(), "M");
		gpuProgram.setUniform(texture == nullptr, "noTexture");
		gpuProgram.setUniform(false, "outline");

		glDisable(GL_CULL_FACE);
		geometry->Draw();
		glEnable(GL_CULL_FACE);
	}


};
