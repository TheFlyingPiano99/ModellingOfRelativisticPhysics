#pragma once

#include "framework.h"
#include "Color.h"
 

class Material {
protected:
	vec3 ka, kd, ks;
	float shininess, wavelength = 435, dopplerShift = 1.0f;


public:
	Material(vec3 _ka, vec3 _kd, vec3 _ks, float _shininess) : ka(_ka), kd(_kd), ks(_ks), shininess(_shininess) {
	}

	void calculateDopplerShift(vec4 observersVelocity, vec4 subjectsVelocity, vec4 observersLocation, vec4 subjectsLocation) {
		float v = RelPhysics::relativeVelocity(observersVelocity, subjectsVelocity);
		//Todo
	}

	void loadOnGPU(GPUProgram& gpuProgram) {
		//gpuProgram.setUniform(ka, "ka");
		gpuProgram.setUniform(waveLengthToRGB((double)(wavelength * dopplerShift)), "kd");
		//gpuProgram.setUniform(ks, "ks");
		//gpuProgram.setUniform(shininess, "shininess");
	}
};