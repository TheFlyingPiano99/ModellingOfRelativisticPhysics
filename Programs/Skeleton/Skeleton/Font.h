#pragma once

#include "framework.h"

/*
* Monospace bitmap fontTexture
*/
class Font {
	Texture* bitmap;
	const int charW, charH, noOfRows, noOfColumns;
	vec3 transparentColor;

public:

	Font(Texture* _bitmap, int _charW = 32, int _charH = 32, int _noOfRows = 8, int _noOfColumns = 16, vec3 _transparent = vec3(0, 0, 0))
		: bitmap(_bitmap), charW(_charW), charH(_charH), noOfRows(_noOfRows), noOfColumns(_noOfColumns), transparentColor(_transparent) {
	}

	void getCharUVs(const char c, vec2& topLeft, vec2& topRight, vec2& bottomRight, vec2& bottomLeft);

	void loadOnGPU(GPUProgram& gpuProgram);
};