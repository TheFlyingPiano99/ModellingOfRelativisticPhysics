#pragma once

#include "framework.h"

#include "Entity.h"
#include "WorldLine.h"
#include "Material.h"
#include "Geometry.h"
#include "Camera.h"
#include "AdvancedTexture.h"


class Object : public Entity
{
	float rotationAngle, rotationSpeed;
	vec3 scale, rotationAxis, translation;
	WorldLine* worldLine = NULL;
	Geometry* geometry;
	Material* material = NULL;
	AdvancedTexture* texture = NULL;
	vec4 locationFV, velocityFV = vec4(0, 0, 0, 1);

public:

	Object(
		vec3 _scale,
		float _rotaionAngle,
		float _rotationSpeed,
		vec3 _translation,
		vec3 _rotationAxis,
		WorldLine* _worldLine,
		Geometry* _geometry,
		Material* _material,
		AdvancedTexture* _texture,
		std::string _name = "",
		std::string _desc = ""
	)
		: Entity(_name, _desc),
		scale(_scale),
		rotationAngle(_rotaionAngle),
		rotationSpeed(_rotationSpeed),
		rotationAxis(_rotationAxis),
		translation(_translation),
		worldLine(_worldLine),
		geometry(_geometry),
		material(_material),
		texture(_texture)
	{
	}

	~Object() {
		if (worldLine != nullptr) {
			delete worldLine;
		}
	}

	mat4 M() {
		return ScaleMatrix(scale) * RotationMatrix(rotationAngle, rotationAxis) * TranslateMatrix(translation);
	}

	mat4 invM() {
		return TranslateMatrix(-translation) * RotationMatrix(-rotationAngle, rotationAxis) * ScaleMatrix(vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
	}

	void Control(float dt, float absoluteTimeSpent) {

	}

	void Animate(float dt, float absoluteTimeSpent) {
		locationFV = worldLine->getLocationAtAbsoluteTime(absoluteTimeSpent);
		velocityFV = worldLine->getVelocityAtAbsoluteTime(absoluteTimeSpent);
		translation = vec3(locationFV.x, locationFV.y, locationFV.z);
		rotationAngle = absoluteTimeSpent * rotationSpeed;
	}

	void Draw(GPUProgram& gpuProgram, Camera& camera) {
		geometry->updateBeforeDraw(camera.getVelocityFV(), camera.getLocationFV(), camera.getHyperplane(), *worldLine);
		material->loadOnGPU(gpuProgram);
		if (texture != nullptr) {
			texture->loadOnGPU(gpuProgram);
		}
		gpuProgram.setUniform(camera.V() * camera.P(), "MVP");
		//gpuProgram.setUniform(M(), "M");
		gpuProgram.setUniform(UnitMatrix(), "invM");

		geometry->Draw(gpuProgram, UnitMatrix(), camera.V(), camera.P());
	}

};

