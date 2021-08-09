#pragma once

#include "framework.h"

#include "Entity.h"
#include "WorldLine.h"
#include "Material.h"
#include "Geometry.h"
#include "Camera.h"


class Object : public Entity
{
	float rotationAngle;
	vec3 scale, rotationAxis, translation;
	WorldLine* worldLine = NULL;
	Geometry* geometry;
	Material* material = NULL;

public:

	Object(
		vec3 _scale,
		float _rotaionAngle,
		vec3 _translation,
		vec3 _rotationAxis,
		WorldLine* _worldLine,
		Geometry* _geometry,
		Material* _material,
		std::string _name = "",
		std::string _desc = ""
	)
		: Entity(_name, _desc),
		scale(_scale),
		rotationAngle(_rotaionAngle),
		rotationAxis(_rotationAxis),
		translation(_translation),
		worldLine(_worldLine),
		geometry(_geometry),
		material(_material)
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
		vec4 trs4 = worldLine->getLocationAtAbsoluteTime(absoluteTimeSpent);
		translation = vec3(trs4.x, trs4.y, trs4.z);
	}

	void Draw(GPUProgram& gpuProgram, Camera& camera) {
		material->loadOnGPU(gpuProgram);
		gpuProgram.setUniform(M() * camera.V() * camera.P(), "MVP");
		gpuProgram.setUniform(M(), "M");

		geometry->Draw(gpuProgram, M(), camera.V(), camera.P());
	}

};

