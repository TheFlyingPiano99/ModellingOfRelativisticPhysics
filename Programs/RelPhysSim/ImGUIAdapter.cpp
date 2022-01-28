#include "ImGUIAdapter.h"

#include "GlobalVariables.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

const char* ImGUIAdapter::Variables::intersectionModeNames[2] = { "Hypercone", "Hyperplane" };
const char* ImGUIAdapter::Variables::dopplerModeNames[3] = { "Full", "Mild", "Off"};
const char* ImGUIAdapter::Variables::transformationModeNames[2] = { "Galilean", "Lorentz" };

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

void ImGUIAdapter::buildCoordinateSelector(Scene& scene)
{
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
	ImGui::Button("X", ImVec2(25, 25));
	ImGui::PopStyleColor();
	if (ImGui::IsItemClicked()) {
		scene.setCameraDirectionMode(RelTypes::DirectionMode::Xlocked);
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 100, 255));
	ImGui::Button("Y", ImVec2(25, 25));
	ImGui::PopStyleColor();
	if (ImGui::IsItemClicked()) {
		scene.setCameraDirectionMode(RelTypes::DirectionMode::Ylocked);
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 100, 255, 255));
	ImGui::Button("Z", ImVec2(25, 25));
	ImGui::PopStyleColor();
	if (ImGui::IsItemClicked()) {
		scene.setCameraDirectionMode(RelTypes::DirectionMode::Zlocked);
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
	ImGui::Button("-X", ImVec2(25, 25));
	ImGui::PopStyleColor();
	if (ImGui::IsItemClicked()) {
		scene.setCameraDirectionMode(RelTypes::DirectionMode::minusXlocked);
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 100, 255));
	ImGui::Button("-Y", ImVec2(25, 25));
	ImGui::PopStyleColor();
	if (ImGui::IsItemClicked()) {
		scene.setCameraDirectionMode(RelTypes::DirectionMode::minusYlocked);
	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 100, 255, 255));
	ImGui::Button("-Z", ImVec2(25, 25));
	ImGui::PopStyleColor();
	if (ImGui::IsItemClicked()) {
		scene.setCameraDirectionMode(RelTypes::DirectionMode::minusZlocked);
	}
	ImGui::SameLine();
	ImGui::Text("Snap cam to axis");
}

void ImGUIAdapter::buildIntersectionModeSelector(Scene& scene)
{
	RelTypes::IntersectionMode& currentMode = scene.getSettings().intersectionMode.get();
	if (ImGui::BeginCombo("Intersection mode", Variables::intersectionModeNames[currentMode])) {
		for (int i = 0; i < 2; i++) {
			bool is_selected =
				(currentMode == i);
			if (ImGui::Selectable(Variables::intersectionModeNames[i], is_selected)) {
				currentMode = (RelTypes::IntersectionMode)i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void ImGUIAdapter::buildDopplerModeSelector(Scene& scene)
{
	RelTypes::DopplerMode& currentMode = scene.getSettings().dopplerMode.get();
	if (ImGui::BeginCombo("Doppler mode", Variables::dopplerModeNames[currentMode])) {
		for (int i = 0; i < 3; i++) {
			bool is_selected =
				(currentMode == i);
			if (ImGui::Selectable(Variables::dopplerModeNames[i], is_selected)) {
				currentMode = (RelTypes::DopplerMode)i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void ImGUIAdapter::buildTransformationModeSelector(Scene& scene)
{
	bool& doLorentz = scene.getSettings().doLorentz.get();
	unsigned int currentMode = (doLorentz) ? 1 : 0;
	if (ImGui::BeginCombo("Transformation", Variables::transformationModeNames[currentMode])) {
		for (int i = 0; i < 2; i++) {
			bool is_selected =
				(currentMode == i);
			if (ImGui::Selectable(Variables::transformationModeNames[i], is_selected)) {
				currentMode = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		doLorentz = (currentMode == 1);
		ImGui::EndCombo();
	}
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
	if (ImGui::Begin("Settings")) {
		ImGui::SetWindowSize(ImVec2(GlobalVariables::windowWidth / 4.0f, GlobalVariables::windowHeight / 2.0f));
		buildCoordinateSelector(scene);
		ImGui::Button("Toggle view mode", ImVec2(150, 50));
		if (ImGui::IsItemClicked()) {
			scene.toggleViewMode();
		}
		buildIntersectionModeSelector(scene);
		buildTransformationModeSelector(scene);
		if (RelTypes::ViewMode::realTime3D == scene.getSettings().viewMode) {
			buildDopplerModeSelector(scene);
		}

		ImGui::Checkbox("Run", &scene.getSettings().running);
		ImGui::Checkbox("Shading", &scene.getSettings().doShading);
		if (RelTypes::ViewMode::realTime3D == scene.getSettings().viewMode && scene.getSettings().doLorentz.get()) {
			ImGui::Checkbox("Simultane boost", &scene.getSettings().simultaneBoost);
		}
		else if (RelTypes::ViewMode::diagram == scene.getSettings().viewMode) {
			ImGui::Checkbox("Display intersectable", &scene.getSettings().displayIntersectable);
			ImGui::Checkbox("Transform to proper frame", &scene.getSettings().transformToProperFrame.get());
			ImGui::Checkbox("Editor mode", &scene.getSettings().editorMode);
		}
		ImGui::End();
	}

}


void ImGUIAdapter::draw()
{
	if (!visible) {
		return;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
