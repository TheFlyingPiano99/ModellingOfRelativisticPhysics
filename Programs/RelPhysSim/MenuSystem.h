#pragma once

#include "GL\glut.h"

static void processMenuEvents(int options) {
	//TODO
}

class MenuSystem {
	int menu = 0;

public:

	MenuSystem() {
		menu = glutCreateMenu(processMenuEvents);
		glutAddMenuEntry("Hyperplane", 0);
		glutAddMenuEntry("Light cone", 1);
		glutAttachMenu(GLUT_MIDDLE_BUTTON);
	}

};
