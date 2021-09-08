#pragma once
#include "framework.h"

/*
* Allows variable, to control interpolated states.
*/
template<typename T>
class InterpolationControl
{	
	T val;
	T prevVal;
	float tMax, t;

public:

	InterpolationControl(float _tMax = 400): t(0.0f), tMax(_tMax) {
	}

	InterpolationControl(T _val, float _tMax = 400) : val(_val), tMax(_tMax), t(0.0f) {
	}

	void animate(const float dt);

/*
	template <typename retType>
	retType interpolate(const T& val1, const T& val2, (retType)(*func1)(void), (retType)(*func2)(void));
*/
	
	// Getters:-----------------------------------------------

	bool isInterpolating() {
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

	// Const:---------------------------------------------------

	const bool isInterpolating() const {
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


template<typename T>
inline void InterpolationControl<T>::animate(const float dt) {
	if (val != prevVal) {
		t += dt;
		if (t >= tMax) {
			prevVal = val;
			t = 0;
		}
	}
}


/*
template<typename T>
template<typename retType>
inline retType InterpolationControl<T>::interpolate(const T& val1, const T& val2, (retType)(*func1)(void), (retType)(*func2)(void))
{
	return retType();
}
*/