#include "SettingsBar.h"

void SettingsBar::updateSettings(const Settings& settings)
{
	clear();
	std::string str = std::string("Intersection: ");
	if (settings.intersectionMode == IntersectionMode::lightCone) {
		str.append("light cone");

	}
	else if (settings.intersectionMode == IntersectionMode::hyperplane) {
		str.append("hyperplane");
	}
	captions.emplace(offsetof(Settings, intersectionMode), Caption::createSmallCameraSpaceCaption(pos + vec2(0.2f, 0), str.c_str()));

	str = std::string("Transformation: ");
	if (settings.doLorentz) {
		str.append("Lorentz");

	}
	else {
		str.append("Galilean");
	}
	captions.emplace(offsetof(Settings, doLorentz), Caption::createSmallCameraSpaceCaption(pos + 3 * vec2(0.2f, 0), str.c_str()));
	if (!settings.running) {
		captions.emplace(offsetof(Settings, running), Caption::createSmallCameraSpaceCaption(pos + 5 * vec2(0.2f, 0), "Paused", vec3(1, 0.4f, 0)));
	}
	if (settings.viewMode == ViewMode::diagram) {
		str = std::string("Frame: ");
		if (settings.transformToProperFrame) {
			str.append("Selected observer's proper frame");
		}
		else {
			str.append("Absolute observer's frame");
		}
		captions.emplace(offsetof(Settings, viewMode), Caption::createSmallCameraSpaceCaption(pos + 7 * vec2(0.2f, 0), str.c_str()));
	}
}

void SettingsBar::clear()
{
	for (const auto& ptr : captions) {
		(*ptr.second.get())->erease();
	}
	captions.clear();
}
