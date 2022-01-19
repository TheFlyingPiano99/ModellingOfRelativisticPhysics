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
		new ImGUIObserver::ObservedVariable<bool>(&variables.running, 
			std::function<void(bool)>([scene](bool _) {
				scene->togglePause();
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
	ImGui::Begin("Relativistic settings");
	ImGui::Text("GUI text.");
	ImGui::Checkbox("Running:", &variables.running);

	//ImGui::SliderFloat("Temporary setting01", &tempX, 0.0f, 10.0f);

	/*
	float minGamma = 0;
	float maxGamma = 10;
	float minExposure = 0;
	float maxExposure = 1;

	ImGui::BeginChild("Atmosphere properties");
	ImGui::Text("HDR");
	ImGui::SliderFloat("Gamma", scene.getPostprocessUnit()->getGamma(), minGamma, maxGamma);
	ImGui::SliderFloat("Exposure", scene.getPostprocessUnit()->getExposure(), minExposure, maxExposure);
	ImGui::EndChild();

	ImGui::BeginChild("Atmosphere properties");
	ImGui::Text("Rayleigh scattering");
	float minRayleigh = 0;
	float maxRayleigh = 0.5f;
	ImGui::SliderFloat("Rayleigh R", &(scene.getPlanet()->getRayleighScattering()->x), minRayleigh, maxRayleigh);
	ImGui::SliderFloat("Rayleigh G", &(scene.getPlanet()->getRayleighScattering()->y), minRayleigh, maxRayleigh);
	ImGui::SliderFloat("Rayleigh B", &(scene.getPlanet()->getRayleighScattering()->z), minRayleigh, maxRayleigh);
	ImGui::EndChild();

	ImGui::BeginChild("Atmosphere properties");
	ImGui::Text("Mie scattering");
	float minMie = 0;
	float maxMie = 0.02;
	ImGui::SliderFloat("Mie", scene.getPlanet()->getMieScattering(), minMie, maxMie);
	ImGui::EndChild();
	*/

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
