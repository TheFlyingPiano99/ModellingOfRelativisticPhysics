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
	Material* diagramMaterial = NULL;
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
		Material* _diagramMaterial,
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
		diagramMaterial(_diagramMaterial),
		texture(_texture)
	{
	}

	~Object() {
		if (worldLine != nullptr) {
			delete worldLine;
		}
	}

	/*
	* Factory for Earth-like object.
	*/
	static Object* createEarth(WorldLine* wrdln);

	/*
	*/
	static Object* createDice(WorldLine* wrdln);

	mat4 M() {
		return ScaleMatrix(scale) * RotationMatrix(rotationAngle, rotationAxis) * TranslateMatrix(translation);
	}

	mat4 invM() {
		return TranslateMatrix(-translation) * RotationMatrix(-rotationAngle, rotationAxis) * ScaleMatrix(vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
	}

	void Control(float dt, float absoluteTimeSpent) {
		//Todo
		;
	}

	void Animate(float dt);

	void Draw(GPUProgram& gpuProgram, Camera& camera);

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);
};

