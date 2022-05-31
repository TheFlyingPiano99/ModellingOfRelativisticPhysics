#include "ImGUIAdapter.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GlobalInclude.h"

const char* ImGUIAdapter::Variables::intersectionModeNames[2] = { "Hypercone", "Hyperplane" };
const char* ImGUIAdapter::Variables::dopplerModeNames[3] = { "Full", "Mild", "Off"};
const char* ImGUIAdapter::Variables::transformationModeNames[2] = { "Galilean", "Lorentz" };

void ImGUIAdapter::initGUI(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init("#version 420");
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
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGUIAdapter::buildCameraButtons(Scene& scene)
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
	ImGui::Button("+", ImVec2(25, 25));
	if (ImGui::IsItemClicked()) {
		scene.zoomCamera(1.0f + 0.01f);
	}
	ImGui::SameLine();
	ImGui::Button("-", ImVec2(25, 25));
	if (ImGui::IsItemClicked()) {
		scene.zoomCamera(1.0f - 0.01f);
	}
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

void ImGUIAdapter::buildObjectTypeSelector(Scene& scene)
{
	unsigned int currentType = scene.getInitialiser()->getObjectType();
	if (ImGui::BeginCombo("Object type", Object::typeNames[currentType])) {
		for (int i = 0; i < 5; i++) {
			bool is_selected =
				(currentType == i);
			if (ImGui::Selectable(Object::typeNames[i], is_selected)) {
				currentType = i;
				scene.getInitialiser()->setObjectType((RelTypes::ObjectType)currentType);
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void ImGUIAdapter::buildWorldLineTypeSelector(Scene& scene)
{
	unsigned int currentType = scene.getInitialiser()->getWorldLineType();
	if (ImGui::BeginCombo("World line type", WorldLine::typeNames[currentType])) {
		for (int i = 0; i < NUMBER_OF_WORLD_LINE_TYPES; i++) {
			bool is_selected =
				(currentType == i);
			if (ImGui::Selectable(WorldLine::typeNames[i], is_selected)) {
				currentType = i;
				scene.getInitialiser()->setWorldLineType((WorldLine::Type)currentType);
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void ImGUIAdapter::preDrawInit()
{
	if (!visible) {
		return;
	}
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

float tempX = 0.0f;

void ImGUIAdapter::configToScene(Scene& scene)
{
	if (!visible) {
		return;
	}
	ImGui::Begin("Settings");
	buildCameraButtons(scene);
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
	ImGui::Checkbox("Path", &scene.getSettings().drawPath);
	if (RelTypes::ViewMode::realTime3D == scene.getSettings().viewMode && scene.getSettings().doLorentz.get()) {
		ImGui::Checkbox("Simultane boost", &scene.getSettings().simultaneBoost);
	}
	else if (RelTypes::ViewMode::diagram == scene.getSettings().viewMode) {
		ImGui::Checkbox("Display intersectable", &scene.getSettings().displayIntersectable);
		ImGui::Checkbox("Transform to proper frame", &scene.getSettings().transformToProperFrame.get());
		ImGui::Checkbox("Editor mode", &scene.getSettings().editorMode);
	}

	if (ImGui::Button("New object", ImVec2(120, 50))) {
		scene.beginCreationSequence();
	}
	ImGui::End();

	//Initialisation window:
	if (scene.getCreationSequence() == CreationSequence::costumize) {
		ImGui::Begin("Initialise object");
		ImGui::InputText("Name", scene.getInitialiser()->name, NAME_BUFFER_SIZE);
		buildObjectTypeSelector(scene);
		buildWorldLineTypeSelector(scene);
		ImGui::InputFloat3("Position at start [m]", &scene.getInitialiser()->positionAtZeroT[0]);
		ImGui::InputFloat3("Velocity at start [c]", &scene.getInitialiser()->velocityAtZeroT[0]);
		if (scene.getInitialiser()->getWorldLineType() == WorldLine::Type::SpiralLine) {
			ImGui::InputFloat3("Center of rotation [m]", &scene.getInitialiser()->centerOfRotation[0]);
		}

		if (ImGui::Button("Finish", ImVec2(120, 50))) {
			scene.finishCreationSequence();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 50))) {
			scene.cancelCreationSequence();
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
