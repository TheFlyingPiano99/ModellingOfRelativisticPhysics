#pragma once

class AdvancedTexture {
	Texture* texture;
	Texture* normalMap;
	Texture* specularMap;

public:

	AdvancedTexture(const char* _texturePath, const char* _normalMapPath, const char* _specularMapPath)
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