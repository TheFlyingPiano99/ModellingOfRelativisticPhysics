#pragma once

#include "Geometry.h"
#include "Camera.h"
#include "Material.h"
#include "Assets.h"
#include "TextureCube.h"

class Background {
public:
	Background(GPUProgram* program) : program(program) {
		geometry = new SphereSurface(120);
		geometry->GenSurface(100, 100);
		vec3 color = vec3(0.1,0.1, 0.1);
		material = new Material(color, color, color, 50);
		material->setGlow(true);
		texture = Assets::getSeaBackgroundTexture();
	}

	~Background() {
		delete geometry;
		delete material;
	}

	void draw(const Camera& camera);

private:
	SphereSurface* geometry;
	Material* material;
	TextureCube* texture;
	GPUProgram* program;

};
