#pragma once

#include <exception>

struct LightspeedExceededException : public std::exception {
	char const* what() const {
		return "Lightspeed exceeded exception";
	}
};

