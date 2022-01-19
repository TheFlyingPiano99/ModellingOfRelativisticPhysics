#pragma once

#include <string>

#include "GL\glew.h"
#include "Geometry.h"
#include "Texture.h"
#include "EnumTypes.h"

class GPUProgram {
	unsigned int shaderProgramId = 0;
	unsigned int vertexShader = 0, geometryShader = 0, fragmentShader = 0;
	bool waitError = true;

	void getErrorInfo(unsigned int handle);

	bool checkShader(unsigned int shader, std::string message);

	bool checkLinking(unsigned int program);

	// Get the address of a GPU uniform variable
	int getLocation(const std::string& name);

public:
	GPUProgram(bool _waitError = true) { shaderProgramId = 0; waitError = _waitError; }

	GPUProgram(const GPUProgram& program);

	void operator=(const GPUProgram& program) {
		if (program.shaderProgramId > 0) printf("\nError: GPU program is not copied on GPU!!!\n");
	}

	unsigned int getId() { return shaderProgramId; }

	bool create(const char* const vertexShaderSource,
		const char* const fragmentShaderSource, const char* const fragmentShaderOutputName,
		const char* const geometryShaderSource = nullptr);

	void Use() { 		// make this program run
		glUseProgram(shaderProgramId);
	}

	void setUniform(int i, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniform1i(location, i);
	}

	void setUniform(float f, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniform1f(location, f);
	}

	void setUniform(const vec2& v, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniform2fv(location, 1, &v.x);
	}

	void setUniform(const vec3& v, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniform3fv(location, 1, &v.x);
	}

	void setUniform(const vec4& v, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniform4fv(location, 1, &v.x);
	}

	void setUniform(const mat4& mat, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, mat);
	}

	void setUniform(const std::vector<vec4>& vds, const std::string& name) {
		int location = getLocation(name);
		if (location >= 0) glUniform4fv(location, vds.size(), &(vds[0].x));
	}

	void setUniform(const Texture& texture, const std::string& samplerName, unsigned int textureUnit = 0) {
		int location = getLocation(samplerName);
		if (location >= 0) {
			glUniform1i(location, textureUnit);
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, texture.textureId);
		}
	}

	~GPUProgram() { if (shaderProgramId > 0) glDeleteProgram(shaderProgramId); }

	void loadOnGPU(const RelTypes::Settings& settings) {
		setUniform(false, "textMode");
		setUniform(settings.dopplerMode.get(), "dopplerMode");

		setUniform(settings.doLorentz.get(), "doLorentz");
		setUniform(settings.doLorentz.isInterpolating(), "interpolateDoLorentz");
		setUniform(settings.doLorentz.getFraction(), "tDoLorentz");

		setUniform(settings.intersectionMode.get(), "intersectionMode");
		setUniform(settings.intersectionMode.isInterpolating(), "interpolateIntersectionMode");
		setUniform(settings.intersectionMode.getFraction(), "tIntersectionMode");

		setUniform(settings.transformToProperFrame.get(), "transformToProperFrame");
		setUniform(settings.transformToProperFrame.isInterpolating(), "interpolateTransformToProperFrame");
		setUniform(settings.transformToProperFrame.getFraction(), "tTransformToProperFrame");

		setUniform(settings.doShading, "doShading");
		setUniform(settings.viewMode, "viewMode");
		setUniform(vec3(0.05f, 0.05f, 0.05f), "La");
		//TODO (Magic const!)

		setUniform(settings.diagramX, "diagramX");
		setUniform(settings.diagramY, "diagramY");
		setUniform(settings.diagramZ, "diagramZ");

		setUniform(settings.simultaneBoost, "simultaneBoost");
	}

};
