#include "GlutCallbacks.h"
#include <map>
#include <queue>

#include "ImGUIAdapter.h"
#include "GlobalVariables.h"
#include "Application.h"
#include "ControlEventManager.h"

// Initialization, getInstance an OpenGL context

static GPUProgram gpuProgram; // vertex and fragment shaders
int windowID = 0;
ImGUIAdapter* guiAdapter;

void onInitialization() {

	// Controls:
	ControlEventManager::getInstance().buildDefaultControlScheme();

	glViewport(0, 0, GlobalVariables::windowWidth, GlobalVariables::windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

	windowID = glutGetWindow();
	
	scene = Scene::getInstance();
	scene->Initialise();

	guiAdapter = new ImGUIAdapter();
	guiAdapter->initGUI();
	guiAdapter->initBindings(scene);
	guiAdapter->setVisible(true);
}

// Window has become invalid: Redraw
void onDisplay() {
	guiAdapter->preDrawInit();
	glClearColor(0, 0, 0.5f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Draw(gpuProgram);
	guiAdapter->configToScene(*scene);
	guiAdapter->draw();
	glutSwapBuffers();

}


bool escDown = false;

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {

	scene->type(key);

	//Modifiers:
	int modifiers = glutGetModifiers();
	int ctrl = modifiers & GLUT_ACTIVE_CTRL;
	int alt = modifiers & GLUT_ACTIVE_ALT;
	int shift = modifiers & GLUT_ACTIVE_SHIFT;
	
	ControlEventManager::getInstance().handlePressedEvents(*scene, key);

	bool windowDestroyed = false;
	if (key == 27) {
		if (!escDown) {
			escDown = true;
			if (scene->askToQuit())
				delete guiAdapter;
				glutDestroyWindow(windowID);
				windowDestroyed = true;
		}
	}
	if (!windowDestroyed) {
		ImGui_ImplGLUT_KeyboardFunc(key, pX, pY);
	}
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
	if (key == 27) {
		escDown = false;
	}
	ImGui_ImplGLUT_KeyboardUpFunc(key, pX, pY);
}

RelTypes::MouseState mouseState;
static float prevCX, prevCY;

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / GlobalVariables::windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / GlobalVariables::windowHeight;


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
	prevCY = cY;
	ImGui_ImplGLUT_MotionFunc(pX, pY);
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / GlobalVariables::windowWidth - 1;
	float cY = 1.0f - 2.0f * pY / GlobalVariables::windowHeight;	// flip y axis

	char * buttonStat;

	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				scene->selectByClick(cX, cY);
				mouseState.mouseLeftDown = true;
			}
			else if (state == GLUT_UP) {
				scene->releaseGrab();
				mouseState.mouseLeftDown = false; mouseState.mouseLeftPrevDown = false;
			}
			break;
		case GLUT_MIDDLE_BUTTON: break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {	//Start dragging camera
				mouseState.mouseRightDown = true;
			}
			else if (state == GLUT_UP) {	//Stop dragging camera
				mouseState.mouseRightDown = false; mouseState.mouseRightPrevDown = false;
			}
			break;
		case 3:			// Mouse wheel scroll
		case 4:			// Mouse wheel scroll

			break;
		default: break;
	}
	ImGui_ImplGLUT_MouseFunc(button, state, pX, pY);
}


static float dt = 1.0f;
static float dtLimit = 3.0f;

static std::vector<float> fpsBuffer;
static float fpsTimer = 0.0f;
static int fpsIterCount = 0;

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
	static float tEnd = 0;	// init only once
	float tStart = tEnd;
	tEnd = glutGet(GLUT_ELAPSED_TIME);

	fpsTimer += tEnd - tStart;
	fpsIterCount++;
	if (fpsTimer > 500) {
		float fps = (float)fpsIterCount / fpsTimer * 1000.0f;
		fpsBuffer.push_back(fps);
		if (fpsBuffer.size() > 50) {
			fpsBuffer.erase(fpsBuffer.begin());
		}

		float sum = 0;
		for (int i = 0; i < fpsBuffer.size(); i++) {
			sum += fpsBuffer[i];
		}
		float averageFPS = sum / (float)fpsBuffer.size();
		glutSetWindowTitle(std::string(WINDOW_TITLE).append("   FPS: ").append(std::to_string(fps))
			.append(" Average FPS: ").append(std::to_string(averageFPS)).c_str());
		fpsTimer = 0.0f;
		fpsIterCount = 0;
	}

	guiAdapter->checkChanges();

	for (float t = tStart; t < tEnd; t += dt) {
		float DT = (dt < tEnd - t) ? dt : tEnd - t;
		scene->Control(DT);
		scene->Animate(DT);
	}
	glutPostRedisplay();
}

