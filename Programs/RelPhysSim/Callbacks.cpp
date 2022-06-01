#include "Callbacks.h"
#include "Scene.h"
#include "ControlEventManager.h"
#include "SingletonManager.h"
#include "GlobalVariables.h"
#include "ImGUIAdapter.h"
#include "Shaders.h"

// Prevents the camera from jumping around when first clicking left click
bool firstClick = true;

static GPUProgram gpuProgram; // vertex and fragment shaders
int windowID = 0;
ImGUIAdapter* guiAdapter;
static Scene* scene = nullptr;
static unsigned int vao;	   // virtual world on the GPU

void Callbacks::setCallbacks(GLFWwindow* window) {
	glfwSetWindowRefreshCallback(window, Callbacks::onWindowRefresh);
	glfwSetWindowCloseCallback(window, Callbacks::onWindowClose);
	glfwSetKeyCallback(window, Callbacks::onKey);
	glfwSetCursorPosCallback(window, Callbacks::onMouseMove);
	glfwSetScrollCallback(window, Callbacks::onMouseScroll);
	glfwSetMouseButtonCallback(window, Callbacks::onMouseClick);

}

void Callbacks::onWindowInit(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	ControlEventManager::getInstance().buildDefaultControlScheme();

	std::string vSourceString, fSourceString;
	// Vertex shader:
	std::ifstream vertexSource;
	vertexSource.open(Shaders::getShaderPath().append("vertexShader.vert"));
	if (vertexSource.is_open()) {
		vSourceString = std::string(std::istreambuf_iterator<char>(vertexSource), std::istreambuf_iterator<char>());
		vertexSource.close();
	}
	else {
		throw RelTypes::CannotLoadShader("vertex shader");
	}
	// Fragment shader:
	std::ifstream fragmentSource;
	fragmentSource.open(Shaders::getShaderPath().append("fragmentShader.frag"));
	if (fragmentSource.is_open()) {
		fSourceString = std::string(std::istreambuf_iterator<char>(fragmentSource), std::istreambuf_iterator<char>());
		fragmentSource.close();
	}
	else {
		throw RelTypes::CannotLoadShader("fragment shader");
	}
	gpuProgram.create(vSourceString.c_str(), fSourceString.c_str(), "outColor");
	scene = Scene::getInstance();
	scene->Initialise();
	guiAdapter = new ImGUIAdapter();
	guiAdapter->initGUI(window);
	guiAdapter->initBindings(scene);
	guiAdapter->setVisible(true);
}


void Callbacks::onWindowRefresh(GLFWwindow* window)
{
	guiAdapter->preDrawInit();
	scene->draw(gpuProgram);
	guiAdapter->configToScene(*scene);
	guiAdapter->draw();
	glfwSwapBuffers(window);
}


void Callbacks::onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool windowDestroyed = false;
	if (scene->isEntryMode()) {
		scene->type(key);
	}
	else if (scene->getCreationSequence() != CreationSequence::costumize) {
		if (GLFW_PRESS == action) {
			ControlEventManager::getInstance().onPress(*Scene::getInstance(), key);
		}
		else if (GLFW_RELEASE == action) {
			ControlEventManager::getInstance().onRelease(*Scene::getInstance(), key);
		}
	}
}


RelTypes::MouseState mouseState;
static float prevCX, prevCY;

void Callbacks::onMouseMove(GLFWwindow* window, double xpos, double ypos)
{

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{

		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (GlobalVariables::windowWidth / 2), (GlobalVariables::windowHeight / 2));
			firstClick = false;
		}

		float cX = 2.0f * xpos / GlobalVariables::windowWidth - 1;	// flip y axis
		float cY = 1.0f - 2.0f * ypos / GlobalVariables::windowHeight;

		if (!mouseState.mouseRightPrevDown && mouseState.mouseRightDown) {
			prevCX = cX;
			prevCY = cY;
		}

		float deltaCX = cX - prevCX;
		float deltaCY = cY - prevCY;

		if (mouseState.mouseRightDown) {
			mouseState.mouseRightPrevDown = true;
			scene->mouseDragged(cX, cY, deltaCX, deltaCY, mouseState);
		}
		if (mouseState.mouseLeftDown) {
			mouseState.mouseLeftPrevDown = true;
			scene->mouseDragged(cX, cY, deltaCX, deltaCY, mouseState);
		}

		scene->mouseMoved(cX, cY);

		prevCX = cX;
		prevCY = cY;		// Fetches the coordinates of the cursor

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		//glfwSetCursorPos(window, (GlobalVariables::windowWidth / 2), (GlobalVariables::windowHeight/ 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}

void Callbacks::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	scene->mouseScrolled(yoffset);
}

void Callbacks::onMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// Convert to normalized device space
	float cX = 2.0f * xpos / GlobalVariables::windowWidth - 1;
	float cY = 1.0f - 2.0f * ypos / GlobalVariables::windowHeight;	// flip y axis
	char* buttonStat;

	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) {
			scene->selectByClick(cX, cY);
			mouseState.mouseLeftDown = true;
		}
		else if (action == GLFW_RELEASE) {
			scene->releaseGrab();
			mouseState.mouseLeftDown = false; mouseState.mouseLeftPrevDown = false;
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE: break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) {	//Start dragging camera
			mouseState.mouseRightDown = true;
		}
		else if (action == GLFW_RELEASE) {	//Stop dragging camera
			mouseState.mouseRightDown = false; mouseState.mouseRightPrevDown = false;
		}
		break;
	default: break;
	}
}

void Callbacks::toggleFullScreen()
{
	GlobalVariables::fullScreenMode = !GlobalVariables::fullScreenMode;
	if (GlobalVariables::fullScreenMode) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(GlobalVariables::window, monitor, 0, 0, mode->width, mode->height, 60);
	}
	else {
		glfwSetWindowMonitor(GlobalVariables::window, nullptr, 50, 50, GlobalVariables::windowWidth, GlobalVariables::windowHeight, 60);
	}
	glfwGetWindowSize(GlobalVariables::window, &GlobalVariables::windowWidth, &GlobalVariables::windowHeight);
	glViewport(0, 0, GlobalVariables::windowWidth, GlobalVariables::windowHeight);
	std::cout << "Screen size: " << GlobalVariables::windowWidth << ", " << GlobalVariables::windowHeight << std::endl;
	
	Scene::getInstance()->onContextResize(GlobalVariables::windowWidth, GlobalVariables::windowHeight);
}


void Callbacks::onWindowClose(GLFWwindow* window)
{
	// Delete all the objects we've created

	Singleton::destroyAllInstances();
}
