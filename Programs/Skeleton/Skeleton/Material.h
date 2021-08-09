#pragma once

#include "framework.h"
#include "Color.h"
#include <iostream>
 

class Material {
protected:
	vec3 ka, kd, ks;
	float shininess, wavelength = 435, dopplerShift = 1.0f;


public:
	Material(vec3 _ka, vec3 _kd, vec3 _ks, float _shininess) : ka(_ka), kd(_kd), ks(_ks), shininess(_shininess) {
	}

	void calculateDopplerShift(vec4 observersVelocity, vec4 subjectsVelocity, vec4 observersLocation, vec4 subjectsLocation) {
		vec4 toSubject4 = subjectsLocation - observersLocation;
		vec3 toSubject3 = vec3(toSubject4.x, toSubject4.y, toSubject4.z);
		vec4 relVelocityVector4 = subjectsVelocity - observersVelocity;
		vec3 relVelocityVector3 = vec3(relVelocityVector4.x, relVelocityVector4.y, relVelocityVector4.z);
		
		float v = RelPhysics::relativeVelocity(observersVelocity, subjectsVelocity);
		std::cout << "v = " << v << std::endl;
		if (dot(toSubject3, relVelocityVector3) < 0.0f) {	// Ha közeledik
			dopplerShift = sqrtf((RelPhysics::speedOfLight - v) / (RelPhysics::speedOfLight + v));
		}
		else { // Ha távolodik
			dopplerShift = sqrtf((RelPhysics::speedOfLight + v) / (RelPhysics::speedOfLight - v));
		}
	}

	void loadOnGPU(GPUProgram& gpuProgram) {
		//gpuProgram.setUniform(ka, "ka");
		gpuProgram.setUniform(waveLengthToRGB((double)(wavelength * dopplerShift)), "kd");
		//gpuProgram.setUniform(ks, "ks");
		//gpuProgram.setUniform(shininess, "shininess");
	}
};