#pragma once

#include "framework.h"
#include "Font.h"
#include "Camera.h"
#include "Geometry.h"

class CaptionAnimation {

};

class Caption {
	unsigned int vao, vbo, noOfVds;
	vec3 pos;
	Font* font;
	float fontSize;
	vec3 color;
	std::string text;
	CaptionAnimation* animation = NULL;
	bool faceCamera = true;

public:

	Caption(vec3 _pos, Font* _font, float _fontSize, vec3 _color, const char* _text)
		: pos(_pos), font(_font), fontSize(_fontSize), color(_color), text(_text)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, norm));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
		genGeometry();
	}

	~Caption() {
		delete font;					// Temp !!!
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	mat4 M(vec3 norm, vec3 prefUp);

	mat4 invM(vec3 norm, vec3 prefUp);

	void Animate();

	void Draw(GPUProgram& gpuProgram, Camera& camera);

	void genGeometry();

	void changeText(const char* str);

	void setPos(vec3 _pos) {
		pos = _pos;
	}
};

