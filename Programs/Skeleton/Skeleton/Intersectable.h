#pragma once

#include "EnumTypes.h"


class Intersectable {
	const IntersectionMode type;

public:

	Intersectable(IntersectionMode _type): type(_type) {

	}

	IntersectionMode getType() {
		return type;
	}

	const IntersectionMode getType() const {
		return type;
	}

};