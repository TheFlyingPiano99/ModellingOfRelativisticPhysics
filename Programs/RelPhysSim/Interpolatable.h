#pragma once
#include "Geometry.h"

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


template<typename ReqType, typename ValType>
inline ValType interpolate(const InterpolationControl<ReqType>& control, 
	const ReqType& req1, 
	const ReqType& req2, 
	const ValType& val1, 
	const ValType& val2)
{
	ValType retVal;
	if (control.isInterpolating()) {
		// Direction of interpolation:
		if (control.getPrev() == req1 && control.get() == req2) {
			return lerp<ValType>(val1, val2, control.getFraction());
		}
		else if (control.getPrev() == req2 && control.get() == req1) {
			return lerp<ValType>(val2, val1, control.getFraction());
		}
	}
	if (control.get() == req1) {	// Time is relative.
		return val1;
	}
	else if (control.get() == req2) {	// Time is relative.
		return val2;
	}
	return ValType();
}
