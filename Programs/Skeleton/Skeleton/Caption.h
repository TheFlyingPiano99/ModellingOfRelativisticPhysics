#pragma once

#include "framework.h"
#include "Font.h"
#include "Camera.h"
#include "Geometry.h"
#include "Assets.h"

class CaptionAnimation {

};

class Caption {
	unsigned int vao, vbo, noOfVds;
	vec3 pos;
	Font* fontTexture;
	float fontSize;
	vec3 color;
	std::string text;
	CaptionAnimation* animation = NULL;
	bool faceCamera = true;

public:

	Caption(vec3 _pos, Font* _font, float _fontSize, vec3 _color, const char* _text)
		: pos(_pos), fontTexture(_font), fontSize(_fontSize), color(_color), text(_text)
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
		delete fontTexture;					// Temp !!!
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	static Caption* createNormalCaption(vec3 pos, const char* text) {
		return new Caption(pos, Assets::getDefaultFont(), 0.03f, vec3(0.9f,1,1), text);
	}

	mat4 M(vec3 norm, vec3 prefUp, float distance);

	mat4 invM(vec3 norm, vec3 prefUp, float distance);

	void Animate();

	void Draw(GPUProgram& gpuProgram, Camera& camera);

	void DrawDiagram(GPUProgram& gpuProgram, Camera& camera);

	void genGeometry();

	void changeText(const char* str);

	void setPos(vec3 _pos) {
		pos = _pos;
	}

	void setFontSize(float size) {
		fontSize = size;
	}

	float getFontSize() {
		return fontSize;
	}

};
