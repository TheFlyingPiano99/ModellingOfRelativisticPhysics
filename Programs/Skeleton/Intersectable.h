#pragma once

#include "EnumTypes.h"


class Intersectable {
	const RelTypes::IntersectionMode type;

public:

	Intersectable(RelTypes::IntersectionMode _type): type(_type) {

	}

	RelTypes::IntersectionMode getType() {
		return type;
	}

	const RelTypes::IntersectionMode getType() const {
		return type;
	}

};