#pragma once

#define WINDOW_TITLE "RelPhysSim"

struct GlobalVariables {
	GlobalVariables() {}

public:
	static unsigned int windowWidth, windowHeight;
	static bool fullScreenMode;
	static const unsigned int shaderWorldLineResolution;	// Hardcoded in shader!
};
