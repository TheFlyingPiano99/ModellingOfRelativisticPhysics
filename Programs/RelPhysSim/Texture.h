#pragma once
#include <vector>
#include <string>

#include "GL/glew.h"
#include "Geometry.h"

class Texture {
	std::vector<vec4> load(std::string pathname, bool transparent, int& width, int& height);
public:
	unsigned int textureId = 0;

	Texture() { textureId = 0; }

	Texture(std::string pathname, bool transparent = false) {
		textureId = 0;
		create(pathname, transparent);
	}

	Texture(int width, int height, const std::vector<vec4>& image, int sampling = GL_LINEAR) {
		textureId = 0;
		create(width, height, image, sampling);
	}

	Texture(const Texture& texture) {
		printf("\nError: Texture resource is not copied on GPU!!!\n");
	}

	void operator=(const Texture& texture) {
		printf("\nError: Texture resource is not copied on GPU!!!\n");
	}

	void create(std::string pathname, bool transparent = false);

	void create(int width, int height, const std::vector<vec4>& image, int sampling = GL_LINEAR);

	~Texture();

	unsigned int getID() {
		return textureId;
	}
};