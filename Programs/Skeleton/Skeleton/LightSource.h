#pragma once

#include "framework.h"

class LightSource {
	vec3 pos;
	vec3 L;
	int index;

public:

	LightSource(vec3 _pos, vec3 _L, int _index): pos(_pos), L(_L), index(_index) {
	}

	void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(pos, "lightPos" + std::to_string(index));
		gpuProgram.setUniform(L, "lightL" + std::to_string(index));
	}
};