#pragma once

#pragma once

#include "Scene.h"
#include "ImGUIObserver.h"
#include "imgui/imgui_impl_glfw.h"

/*
* Singleton object
*/
class ImGUIAdapter
{
	bool visible = false;
	ImGuiIO io;

	struct Variables {
		bool paused;
		float tempX;
		static const char* intersectionModeNames[2];
		static const char* dopplerModeNames[3];
		static const char* transformationModeNames[2];
	} variables;

	ImGUIObserver guiObserver;

	void destroyGUI();

	void buildCameraButtons(Scene& scene);
	void buildIntersectionModeSelector(Scene& scene);
	void buildDopplerModeSelector(Scene& scene);
	void buildTransformationModeSelector(Scene& scene);
	void buildObjectTypeSelector(Scene& scene);
	void buildWorldLineTypeSelector(Scene& scene);

public:
	ImGUIAdapter() {}

	~ImGUIAdapter() {
		destroyGUI();
	}

	/*
	* Should be called after application start.
	*/
	void initGUI(GLFWwindow* window);

	/*
	* Buid bindings between guid variables and the Scene.
	*/
	void initBindings(Scene* scene);

	/*
	* Shoud be called to observe changes of gui variables and perform callback function calls.
	*/
	void checkChanges();

	/*
	* Shoudl be called every time before drawing scene.
	*/
	void preDrawInit();

	/*
	* Should be called before draw.
	*/
	void configToScene(Scene& scene);

	/*
	* Renders gui on scene.
	*/
	void draw();

	void setVisible(bool b) {
		visible = b;
	}

	bool isVisible() {
		return visible;
	}

};

