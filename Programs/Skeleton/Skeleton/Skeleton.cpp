#include "framework.h"

#include "Shaders.h"
#include "Scene.h"
#include <iostream>


GPUProgram gpuProgram; // vertex and fragment shaders
unsigned int vao;	   // virtual world on the GPU

Scene* scene = NULL;


// Initialization, create an OpenGL context
void onInitialization() {

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	scene = new Scene();
	scene->Create();

	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}

// Window has become invalid: Redraw
void onDisplay() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Draw(gpuProgram);

	glutSwapBuffers();

}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
	if (key == 'c') {
		scene->toggleCurrentObserver();
		glutPostRedisplay();         // if d, invalidate display, i.e. redraw
	}
	if (key == ' ') {
		scene->togglePause();
		glutPostRedisplay();         // if d, invalidate display, i.e. redraw
	}
	if (key == '=') {
		scene->zoomCamera(1.5f);
	}
	if (key == '-') {
		scene->zoomCamera(0.75f);
	}
	if (key == 'd') {
		scene->toggleDoppler();
	}
	if (key == 'r') {
		scene->windTime(-10);
	}
	if (key == 'f') {
		scene->windTime(10);
	}
	if (key == 'i') {
		scene->toggleIntersectionType();
	}
	if (key == 'l') {
		scene->toggleLorentzTransformation();
	}


}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
}

static bool down = false;
static bool prevDown = false;
static float prevCX, prevCY;

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;


	if (!prevDown && down) {
		prevCX = cX;
		prevCY = cY;
	}

	float deltaX = cX - prevCX;
	float deltaY = cY - prevCY;

	if (down) {
		prevDown = true;
		scene->moveCamera(deltaX, deltaY);
		glutPostRedisplay();
	}

	prevCX = cX;
	prevCY = cY;
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;

	char * buttonStat;

	switch (button) {
		case GLUT_LEFT_BUTTON: break;
		case GLUT_MIDDLE_BUTTON: break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {	//Start dragging camera
				down = true;
			}
			else if (state == GLUT_UP) {	//Stop dragging camera
				down = false; prevDown = false;
			}
			break;
		case 3:			// Mouse wheel scroll
		case 4:			// Mouse wheel scroll

			break;
		default: break;
	}

}


static float dt = 0.001;

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
	static float tEnd = 0;
	float tStart = tEnd;
	tEnd = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	for (float t = tStart; t < tEnd; t += dt) {
		float DT = (dt < tEnd - t) ? dt : tEnd - t;
		scene->Control(DT);
		scene->Animate(DT);
	}
	glutPostRedisplay();
}
