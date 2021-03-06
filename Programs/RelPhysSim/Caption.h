#pragma once

#include <vector>
#include <memory>

#include "Font.h"
#include "Camera.h"
#include "Geometry.h"
#include "Assets.h"

class CaptionAnimation {

};

class Caption {
	std::shared_ptr<Caption*> sharedThis;
	unsigned int vao, vbo, noOfVds;
	vec3 pos;
	Font* fontTexture;
	float fontSize;
	vec3 color;
	std::string text;
	CaptionAnimation* animation = NULL;
	bool faceCamera = true;
	bool visible = true;
	bool cameraSpace = false;		// Whether the position should be interpereted in camera space.
	
	static void (*pushCaption)(std::shared_ptr<Caption*>);
	static void (*ereaseCaption)(std::shared_ptr<Caption*>);

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
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, norm));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
		genGeometry();
		sharedThis = std::make_shared<Caption*>(this);
		pushCaption(sharedThis);
	}
	Caption() {};

	mat4 getModellMatrix(vec2 cPos, vec3 preferedUp, float asp) const;

	void genGeometry();

public:

	static void setPushCaptionFunction(void (*func)(std::shared_ptr<Caption*>)) {
		pushCaption = func;
	}

	static void setEreaseCaptionFunction(void (*func)(std::shared_ptr<Caption*>)) {
		ereaseCaption = func;
	}

	void erease() {
		setVisible(false);
		ereaseCaption(sharedThis);
	}

	std::shared_ptr<Caption*> getSharedPtr() {
		return sharedThis;
	}

	~Caption() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	/*
	* Creates a new Caption object. Registers the objects to the scene and returns a pointer to the object.
	*/
	static std::shared_ptr<Caption*> createNormalCaption(vec3 pos, const char* text, vec3 _color = vec3(1, 1, 1)) {
		Caption* caption = new Caption(pos, Assets::getDefaultFont(), 0.03f, _color, text);
		return caption->getSharedPtr();
	}

	static std::shared_ptr<Caption*> createSmallCaption(vec3 pos, const char* text, vec3 _color = vec3(1, 1, 1)) {
		Caption* caption = new Caption(pos, Assets::getDefaultFont(), 0.015f, _color, text);
		return caption->getSharedPtr();
	}

	static std::shared_ptr<Caption*> createNormalCameraSpaceCaption(vec2 sPos, const char* text, vec3 _color = vec3(1, 1, 1)) {

		Caption* caption = new Caption(vec3(sPos.x, sPos.y, 0), Assets::getDefaultFont(), 0.03f, _color, text);
		caption->setCameraSpace(true);

		return caption->getSharedPtr();
	}

	static std::shared_ptr<Caption*> createSmallCameraSpaceCaption(vec2 sPos, const char* text, vec3 _color = vec3(1, 1, 1)) {

		Caption* caption = new Caption(vec3(sPos.x, sPos.y, 0), Assets::getDefaultFont(), 0.023f, _color, text);
		caption->setCameraSpace(true);
		return caption->getSharedPtr();
	}

	void animate();

	void draw(GPUProgram& gpuProgram, const Camera& camera) const;

	void changeText(const char* str);

	const vec3 getPos() const {
		return pos;
	}

	void setPos(vec3 _pos) {
		pos = _pos;
	}

	void setFontSize(float size) {
		fontSize = size;
	}

	const float getFontSize() const {
		return fontSize;
	}

	void setVisible(bool b) {
		visible = b;
	}

	void setColor(vec3 _color) {
		color = _color;
	}

	const vec3 getColor() const {
		return color;
	}

	void setCameraSpace(bool b) {
		cameraSpace = b;
	}
};
