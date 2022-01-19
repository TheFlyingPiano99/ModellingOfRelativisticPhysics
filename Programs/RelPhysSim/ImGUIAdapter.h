#pragma once

#pragma once

#include "imgui/imgui_impl_glut.h"
#include "Scene.h"

/*
* Singleton object
*/
class ImGUIAdapter
{
	ImGuiIO io;

	bool visible = false;

public:

	void initGUI();

	void destroyGUI();

	void preDrawInit();

	void configToScene(Scene& scene);

	void draw();

	void setVisible(bool b) {
		visible = b;
	}

	bool isVisible() {
		return visible;
	}

};

