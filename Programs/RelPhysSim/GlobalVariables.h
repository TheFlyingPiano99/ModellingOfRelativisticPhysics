#pragma once
#include <GLFW/glfw3.h>

#define WINDOW_TITLE "RelPhysSim"

struct GlobalVariables {
	GlobalVariables() = default;

	static GLFWwindow* window;
	static int windowWidth;
	static int windowHeight;
	static bool fullScreenMode;
	static const unsigned int shaderWorldLineResolution;	// Hardcoded in shader!
};
