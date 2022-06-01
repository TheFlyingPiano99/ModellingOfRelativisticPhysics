#include "GPUProgram.h"

void GPUProgram::getErrorInfo(unsigned int handle) { // shader error report
	int logLen, written;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0) {
		std::string log(logLen, '\0');
		glGetShaderInfoLog(handle, logLen, &written, &log[0]);
		printf("Shader log:\n%s", log.c_str());
		if (waitError) getchar();
	}
}

bool GPUProgram::checkShader(unsigned int shader, std::string message) { // check if shader could be compiled
	int OK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
	if (!OK) {
		printf("%s!\n", message.c_str());
		getErrorInfo(shader);
		return false;
	}
	return true;
}

bool GPUProgram::checkLinking(unsigned int program) { 	// check if shader could be linked
	int OK;
	glGetProgramiv(program, GL_LINK_STATUS, &OK);
	if (!OK) {
		printf("Failed to link shader program!\n");
		getErrorInfo(program);
		return false;
	}
	return true;
}

// Get the address of a GPU uniform variable

int GPUProgram::getLocation(const std::string& name) {
	int location = glGetUniformLocation(shaderProgramId, name.c_str());
	if (location < 0) printf("uniform %s cannot be set\n", name.c_str());
	return location;
}

GPUProgram::GPUProgram(const GPUProgram& program) {
	if (program.shaderProgramId > 0) printf("\nError: GPU program is not copied on GPU!!!\n");
}

bool GPUProgram::create(const char* const vertexShaderSource, const char* const fragmentShaderSource, const char* const fragmentShaderOutputName, const char* const geometryShaderSource)
{
	// Create vertex shader from string
	if (vertexShader == 0) vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (!vertexShader) {
		printf("Error in vertex shader creation\n");
		exit(1);
	}
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	if (!checkShader(vertexShader, "Vertex shader error")) return false;

	// Create geometry shader from string if given
	if (geometryShaderSource != nullptr) {
		if (geometryShader == 0) geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		if (!geometryShader) {
			printf("Error in geometry shader creation\n");
			exit(1);
		}
		glShaderSource(geometryShader, 1, (const GLchar**)&geometryShaderSource, NULL);
		glCompileShader(geometryShader);
		if (!checkShader(geometryShader, "Mesh shader error")) return false;
	}

	// Create fragment shader from string
	if (fragmentShader == 0) fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragmentShader) {
		printf("Error in fragment shader creation\n");
		exit(1);
	}

	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	if (!checkShader(fragmentShader, "Fragment shader error")) return false;

	shaderProgramId = glCreateProgram();
	if (!shaderProgramId) {
		printf("Error in shader program creation\n");
		exit(1);
	}
	glAttachShader(shaderProgramId, vertexShader);
	glAttachShader(shaderProgramId, fragmentShader);
	if (geometryShader > 0) glAttachShader(shaderProgramId, geometryShader);

	// Connect the fragmentColor to the frame buffer memory
	glBindFragDataLocation(shaderProgramId, 0, fragmentShaderOutputName);	// this output goes to the frame buffer memory

																			// program packaging
	glLinkProgram(shaderProgramId);
	if (!checkLinking(shaderProgramId)) return false;

	// make this program run
	glUseProgram(shaderProgramId);
	return true;
}
