#include "SettingsBar.h"

void SettingsBar::updateSettings(const RelTypes::Settings& settings)
{
	clear();
	std::string str = std::string("[i] Intersection: ");
	if (settings.intersectionMode.get() == RelTypes::IntersectionMode::lightCone) {
		str.append("light cone");

	}
	else if (settings.intersectionMode.get() == RelTypes::IntersectionMode::hyperplane) {
		str.append("hyperplane");
	}
	captions.emplace(offsetof(RelTypes::Settings, intersectionMode), Caption::createSmallCameraSpaceCaption(pos + vec2(0.2f, 0), str.c_str()));

	str = std::string("[l] Transformation: ");
	if (settings.doLorentz.get()) {
		str.append("Lorentz");

	}
	else {
		str.append("Galilean");
	}
	captions.emplace(offsetof(RelTypes::Settings, doLorentz), Caption::createSmallCameraSpaceCaption(pos + 3 * vec2(0.2f, 0), str.c_str()));
	if (!settings.running) {
		captions.emplace(offsetof(RelTypes::Settings, running), Caption::createSmallCameraSpaceCaption(pos + 5 * vec2(0.2f, 0), "Paused", vec3(1, 0.4f, 0)));
	}
	if (settings.viewMode == RelTypes::ViewMode::diagram) {		// Diagram
		str = std::string("[t] Frame: ");
		if (settings.transformToProperFrame.get()) {
			str.append("Selected observer");
		}
		else {
			str.append("Absolute observer");
		}
		captions.emplace(offsetof(RelTypes::Settings, transformToProperFrame), Caption::createSmallCameraSpaceCaption(pos + 7 * vec2(0.2f, 0), str.c_str()));

		str = std::string("[v] View mode: Diagram");
		captions.emplace(offsetof(RelTypes::Settings, viewMode), Caption::createSmallCameraSpaceCaption(pos + 9 * vec2(0.2f, 0), str.c_str()));
		if (settings.editorMode) {
			captions.emplace(offsetof(RelTypes::Settings, editorMode), Caption::createSmallCameraSpaceCaption(pos + 5 * vec2(0.2f, 0) + vec2(0, 0.1f), "Editing", vec3(0.4f, 1, 0)));
		}
	}
	else {												// RealTime3D
		str = std::string("[v] View mode: Real time 3D");
		captions.emplace(offsetof(RelTypes::Settings, viewMode), Caption::createSmallCameraSpaceCaption(pos + 9 * vec2(0.2f, 0), str.c_str()));
	}
}

void SettingsBar::clear()
{
	for (const auto& ptr : captions) {
		(*ptr.second.get())->erease();
	}
	captions.clear();
}
