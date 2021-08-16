#pragma once

#include <exception>

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
