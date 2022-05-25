#pragma once

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
		geometry = new SphereSurface(120);
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



	void draw(GPUProgram& gpuProgram, const Camera& camera);

	void drawDiagram(GPUProgram& gpuProgram, const Camera& camera);


};
