#pragma once

#include "framework.h"
#include "Color.h"
#include <iostream>
 

class Material {
protected:
	vec3 ka, kd, ks;
	float shininess, wavelength = 435;
	bool glow = false;
	float transparency = 1.0f;


public:
	Material(vec3 _ka, vec3 _kd, vec3 _ks, float _shininess) : ka(_ka), kd(_kd), ks(_ks), shininess(_shininess) {
	}

	void setGlow(bool _glow) {
		glow = _glow;
	}

	void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(ka, "ka");
		gpuProgram.setUniform(kd, "kd");
		gpuProgram.setUniform(ks, "ks");
		gpuProgram.setUniform(shininess, "shininess");
		gpuProgram.setUniform(glow, "glow");
		gpuProgram.setUniform(transparency, "transparency");
	}
};