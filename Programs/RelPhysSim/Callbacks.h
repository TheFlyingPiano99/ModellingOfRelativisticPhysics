#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace Callbacks
{
	void setCallbacks(GLFWwindow* window);

	void onWindowInit(GLFWwindow* window);
	void onWindowRefresh(GLFWwindow* window);
	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	void onWindowClose(GLFWwindow* window);
	void onMouseMove(GLFWwindow* window, double xpos, double ypos);
	void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	void onMouseClick(GLFWwindow* window, int button, int action, int mods);

	void toggleFullScreen();
};

