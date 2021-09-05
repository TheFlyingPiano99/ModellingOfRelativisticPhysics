#pragma once
#include "framework.h"

/*
* Allows variable, to control interpolated states.
*/
template<typename T>
class Interpolatable
{	
	T val;
	T prevVal;
	float tMax, t;

public:

	Interpolatable(float _tMax = 300): t(0.0f), tMax(_tMax) {
	}

	Interpolatable(T _val, float _tMax = 300) : val(_val), tMax(_tMax), t(0.0f) {
	}

	void animate(const float dt) {
		if (val != prevVal) {
			t += dt;
			if (t >= tMax) {
				prevVal = val;
				t = 0;
			}
		}
	}

	bool interpolating() {
		return prevVal != val;
	}

	void set(const T _val) {
		val = _val;
	}

	float getT() {
		return t;
	}

	float getFraction() {
		return invLerp<float>(0.0f, tMax, t);
	}

	T& get() {
		return val;
	}

	T& getPrev() {
		return prevVal;
	}

	const bool interpolating() const {
		return prevVal != val;
	}

	const float getT() const {
		return t;
	}

	const float getFraction() const {
		return invLerp<float>(0.0f, tMax, t);
	}

	const T& get() const {
		return val;
	}

	const T& getPrev() const {
		return prevVal;
	}
};

