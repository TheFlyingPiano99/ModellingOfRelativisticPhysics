#pragma once

#include <string>

namespace Shaders {

	static std::string getShaderPath() {
		std::string path;
		for (int n = 0; n < 5; n++) {
			for (int i = 0; i < n; i++) {
				path.append("../");
			}
			path.append("Resources/shaders/");
			std::filesystem::path p = std::filesystem::path(path.c_str());
			if (std::filesystem::is_directory(p)) {
				return path;
			}
			path.clear();
		}
		throw std::exception("Resources/shaders/ folder not found!");
	}

};


