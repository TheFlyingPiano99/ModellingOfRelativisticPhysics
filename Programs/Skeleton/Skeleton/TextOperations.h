#pragma once

#include "framework.h"


class CaptionAnimation {

};

class Caption {
	vec2 pos;
	void* font;
	vec3 color;
	const unsigned char* text;
	CaptionAnimation* animation = NULL;

public:

	Caption(vec2 _pos, void* _font, vec3 _color, const char* _text)
		: pos(_pos), font(_font), color(_color)
	{
		text = reinterpret_cast<const unsigned char*>(_text);
	}

	void Animate();

	void Draw();
};

