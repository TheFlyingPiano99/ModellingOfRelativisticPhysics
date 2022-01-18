#pragma once

#include "framework.h"

static void processMenuEvents(int options) {
	switch (options)
	{
	default:
		break;
	}
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
