#include "ImGUIAdapter.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

void ImGUIAdapter::initGUI() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGLUT_Init();

	ImGui_ImplOpenGL3_Init("#version 330");

}

void ImGUIAdapter::initBindings(Scene* scene)
{
	guiObserver.addBinding(
		new ImGUIObserver::ObservedVariable<bool>(&scene->getSettings().running,
			std::function<void(bool)>([scene](bool _) {
				})
			)
	);
	guiObserver.addBinding(
		new ImGUIObserver::ObservedVariable<bool>(&scene->getSettings().simultaneBoost,
			std::function<void(bool)>([scene](bool _) {
				})
			)
	);

}

void ImGUIAdapter::checkChanges()
{
	guiObserver.checkChanges();
}

void ImGUIAdapter::destroyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}

void ImGUIAdapter::preDrawInit()
{
	if (!visible) {
		return;
	}
	int w, h;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	ImGuiIO* io = &ImGui::GetIO();
	if (io == nullptr) {
		return;
	}
	io->DisplaySize = ImVec2((float)w, (float)h);
	//io.DisplayFramebufferScale = ImVec2();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();
}

float tempX = 0.0f;

void ImGUIAdapter::configToScene(Scene& scene)
{
	if (!visible) {
		return;
	}
	ImGui::Begin("Settings");
	ImGui::Checkbox("Running", &scene.getSettings().running);
	ImGui::Checkbox("Lorentz", &scene.getSettings().doLorentz.get());
	if (RelTypes::ViewMode::realTime3D == scene.getSettings().viewMode) {
		ImGui::Checkbox("Shading", &scene.getSettings().doShading);
		ImGui::Checkbox("Simultane boost", &scene.getSettings().simultaneBoost);
	}
	else if (RelTypes::ViewMode::diagram == scene.getSettings().viewMode) {
		ImGui::Checkbox("Display intersectable", &scene.getSettings().displayIntersectable);
		ImGui::Checkbox("Editor mode", &scene.getSettings().editorMode);
		ImGui::Checkbox("Transform to proper frame", &scene.getSettings().transformToProperFrame.get());
	}

	ImGui::End();
}


void ImGUIAdapter::draw()
{
	if (!visible) {
		return;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
