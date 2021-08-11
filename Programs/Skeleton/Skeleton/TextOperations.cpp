#include "TextOperations.h"

void Caption::Animate()
{
	if (animation != nullptr) {
		//Todo
	}
}

void Caption::Draw()
{	
	glColor3f(color.x, color.y, color.z);
	glRasterPos2f(pos.x, pos.y);
	glutBitmapString(font, text);

	// Draw blue text at screen coordinates (100, 120), where (0, 0) is the top-left of the
	// screen in an 18-point Helvetica font
	glRasterPos2i(600, 120);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)"text to render");
	
	
}
