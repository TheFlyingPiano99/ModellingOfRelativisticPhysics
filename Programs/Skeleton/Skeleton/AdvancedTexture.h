#pragma once

#include "framework.h"


class AdvancedTexture {
	Texture* texture;
	Texture* normalMap;
	Texture* specularMap;

public:

	AdvancedTexture(char* _texturePath, char* _normalMapPath, char* _specularMapPath)
		: texture(new Texture(_texturePath)),
		normalMap(new Texture(_normalMapPath)),
		specularMap(new Texture(_specularMapPath))
	{}

	~AdvancedTexture() {
		delete texture;
		delete normalMap;
		delete specularMap;
	}

	void loadOnGPU(GPUProgram& gpuProgram) {
		gpuProgram.setUniform(*texture, "textureUnit");
		//gpuProgram.setUniform(*normalMap, "normalMapUnit");
		//gpuProgram.setUniform(*specularMap, "specularMapUnit");
	}

	int getTextureId() {
		return texture->textureId;
	}

};