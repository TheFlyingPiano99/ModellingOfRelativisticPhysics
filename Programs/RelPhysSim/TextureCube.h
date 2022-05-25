#pragma once
#include <stb/stb_image.h>
#include <vector>
#include "Geometry.h"
#include <string>
#include "GPUProgram.h"

class TextureCube
{
	ivec2 dimensions;

public:
	friend class FBO;

	GLuint ID;
	GLuint unit;
	const char* type;

	TextureCube(std::vector<std::string>& images, GLuint unit);

	~TextureCube() {
		this->Delete();
	}

	// Assigns a texture unit to a texture
	void texUnit(GPUProgram& program, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	const ivec2 getDimensions() {
		return dimensions;
	}

private:
	std::vector<vec4> load(std::string pathname, bool transparent, int& width, int& height);

};
