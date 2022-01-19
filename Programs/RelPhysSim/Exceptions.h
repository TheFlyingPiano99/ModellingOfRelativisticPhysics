#pragma once

#include <exception>
#include <string>

namespace RelTypes {

	struct LightspeedExceededException : public std::exception {
		char const* what() const {
			return "Lightspeed exceeded exception";
		}
	};

	struct DoesNotIntersectException : public std::exception {
		char const* what() const {
			return "Does not intersect exception";
		}
	};

	struct CannotLoadGeometry : public std::exception {
		char const* what() const {
			return "Cannot load geometry exception";
		}
	};

	struct CannotLoadShader : public std::exception {
		std::string shaderName;
		CannotLoadShader(std::string _shaderName): shaderName(shaderName) {
		}

		char const* what() const {
			return std::string(std::string("Cannot load shader:\"") + shaderName + std::string("\" exception")).c_str();
		}
	};

	struct CannotLoadScene : public std::exception {
		char const* what() const {
			return "Cannot load scene exception";
		}
	};

}

