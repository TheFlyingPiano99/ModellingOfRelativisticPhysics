#pragma once

#include <math.h>

struct vec2 {
	float x, y;

	vec2(float x0 = 0, float y0 = 0) { x = x0; y = y0; }
	vec2 operator*(float a) const { return vec2(x * a, y * a); }
	vec2 operator/(float a) const { return vec2(x / a, y / a); }
	vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator-(const vec2& v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator*(const vec2& v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator-() const { return vec2(-x, -y); }
};

inline float dot(const vec2& v1, const vec2& v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

inline float length(const vec2& v) { return sqrtf(dot(v, v)); }

inline vec2 normalize(const vec2& v) { return v * (1 / length(v)); }

inline vec2 operator*(float a, const vec2& v) { return vec2(v.x * a, v.y * a); }

struct vec3 {
	float x, y, z;

	vec3(float x0 = 0, float y0 = 0, float z0 = 0) { x = x0; y = y0; z = z0; }
	vec3(vec2 v) { x = v.x; y = v.y; z = 0; }

	float& operator[](int j) { return *(&x + j); }
	float operator[](int j) const { return *(&x + j); }
	vec3 operator*(float a) const { return vec3(x * a, y * a, z * a); }
	vec3 operator/(float a) const { return vec3(x / a, y / a, z / a); }
	vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 operator-()  const { return vec3(-x, -y, -z); }
};

inline float dot(const vec3& v1, const vec3& v2) { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

inline float length(const vec3& v) { return sqrtf(dot(v, v)); }

inline vec3 normalize(const vec3& v) { return v * (1 / length(v)); }

inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

inline vec3 operator*(float a, const vec3& v) { return vec3(v.x * a, v.y * a, v.z * a); }

//--------------------------
/*
* x, y, z .. used for space dimensions
* w ... used for time
*/
struct vec4 {
	//--------------------------
	float x, y, z, w;

	vec4(float x0 = 0, float y0 = 0, float z0 = 0, float w0 = 0) { x = x0; y = y0; z = z0; w = w0; }
	float& operator[](int j) { return *(&x + j); }
	float operator[](int j) const { return *(&x + j); }

	vec4 operator*(float a) const { return vec4(x * a, y * a, z * a, w * a); }
	vec4 operator/(float d) const { return vec4(x / d, y / d, z / d, w / d); }
	vec4 operator+(const vec4& v) const { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	vec4 operator-(const vec4& v)  const { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	vec4 operator*(const vec4& v) const { return vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	void operator+=(const vec4 right) { x += right.x; y += right.y; z += right.z, w += right.z; }
};

inline float dot(const vec4& v1, const vec4& v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

inline vec4 operator*(float a, const vec4& v) {
	return vec4(v.x * a, v.y * a, v.z * a, v.w * a);
}

//---------------------------
struct mat4 { // row-major matrix 4x4
//---------------------------
	vec4 rows[4];
public:

	mat4() {}
	mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		rows[0][0] = m00; rows[0][1] = m01; rows[0][2] = m02; rows[0][3] = m03;
		rows[1][0] = m10; rows[1][1] = m11; rows[1][2] = m12; rows[1][3] = m13;
		rows[2][0] = m20; rows[2][1] = m21; rows[2][2] = m22; rows[2][3] = m23;
		rows[3][0] = m30; rows[3][1] = m31; rows[3][2] = m32; rows[3][3] = m33;
	}
	mat4(vec4 it, vec4 jt, vec4 kt, vec4 ot) {
		rows[0] = it; rows[1] = jt; rows[2] = kt; rows[3] = ot;
	}

	vec4& operator[](int i) { return rows[i]; }
	vec4 operator[](int i) const { return rows[i]; }
	operator float* () const { return (float*)this; }
};

inline vec4 operator*(const vec4& v, const mat4& mat) {
	return v[0] * mat[0] + v[1] * mat[1] + v[2] * mat[2] + v[3] * mat[3];
}

inline float length(const vec4& v) { return sqrtf(dot(v, v)); }

inline vec4 normalize(const vec4& v) { return v * (1.0f / length(v)); }

inline mat4 operator*(const mat4& left, const mat4& right) {
	mat4 result;
	for (int i = 0; i < 4; i++) result.rows[i] = left.rows[i] * right;
	return result;
}

inline mat4 UnitMatrix() {
	return mat4(vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1));
}


inline mat4 TranslateMatrix(vec3 t) {
	return mat4(vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(t.x, t.y, t.z, 1));
}

inline mat4 ScaleMatrix(vec3 s) {
	return mat4(vec4(s.x, 0, 0, 0),
		vec4(0, s.y, 0, 0),
		vec4(0, 0, s.z, 0),
		vec4(0, 0, 0, 1));
}

inline mat4 RotationMatrix(float angle, vec3 w) {
	float c = cosf(angle), s = sinf(angle);
	w = normalize(w);
	return mat4(vec4(c * (1 - w.x * w.x) + w.x * w.x, w.x * w.y * (1 - c) + w.z * s, w.x * w.z * (1 - c) - w.y * s, 0),
		vec4(w.x * w.y * (1 - c) - w.z * s, c * (1 - w.y * w.y) + w.y * w.y, w.y * w.z * (1 - c) + w.x * s, 0),
		vec4(w.x * w.z * (1 - c) + w.y * s, w.y * w.z * (1 - c) - w.x * s, c * (1 - w.z * w.z) + w.z * w.z, 0),
		vec4(0, 0, 0, 1));
}

inline mat4 Transpose(mat4 getModellMatrix) {
	mat4 transposed;
	transposed[0][0] = getModellMatrix[0][0];
	transposed[1][0] = getModellMatrix[0][1];
	transposed[2][0] = getModellMatrix[0][2];
	transposed[3][0] = getModellMatrix[0][3];

	transposed[0][1] = getModellMatrix[1][0];
	transposed[1][1] = getModellMatrix[1][1];
	transposed[2][1] = getModellMatrix[1][2];
	transposed[3][1] = getModellMatrix[1][3];

	transposed[0][2] = getModellMatrix[2][0];
	transposed[1][2] = getModellMatrix[2][1];
	transposed[2][2] = getModellMatrix[2][2];
	transposed[3][2] = getModellMatrix[2][3];

	transposed[0][3] = getModellMatrix[3][0];
	transposed[1][3] = getModellMatrix[3][1];
	transposed[2][3] = getModellMatrix[3][2];
	transposed[3][3] = getModellMatrix[3][3];
	return transposed;
}

inline vec2 solveQuadraticFunction(float a, float b, float c, int& noOfRealSolutions) {
	if (a == 0.0f && b != 0.0f) {
		noOfRealSolutions = 1;
		return vec2(-c / b, -c / b);
	}
	float discriminant = b * b - 4 * a * c;
	vec2 results;
	if (discriminant > 0.0f) {
		noOfRealSolutions = 2;
		results.x = (-b + sqrtf(discriminant)) / (2 * a);
		results.y = (-b - sqrtf(discriminant)) / (2 * a);
	}
	else if (discriminant == 0.0f) {
		noOfRealSolutions = 1;
		results.x = (-b + sqrtf(discriminant)) / (2 * a);
		results.y = results.x;
	}
	else {
		noOfRealSolutions = 0;
	}
	return results;
}

inline float LorentzianProduct(vec4 v1, vec4 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z - v1.w * v2.w;
}

template<typename T>
inline T lerp(const T startVal, const T endVal, const float t) {
	return startVal * (1 - t) + endVal * t;
}

template<typename T>
inline float invLerp(const T startVal, const T endVal, const T val) {
	return (val - startVal) / (endVal - startVal);
}
template<typename TIn, typename TOut>
inline TOut remap(const TIn startIn, const TIn endIn, const TOut startOut, TOut endOut, TIn val) {
	float t = invLerp(startIn, endIn, val);
	return lerp(startOut, endOut, t);
}

/*
//Transformaciok eloadasbol:
vec4 qmul(vec4 q1, vec4 q2) {
	vec3 d1(q1.x, q1.y, q1.z), d2(q2.x, q2.y, q2.z);
	vec3 v = d2 * q1.w + d1 * q2.w + cross(d1, d2);
	return vec4(v.x, v.y, v.z, q1.w * q2.w - dot(d1, d2));
}
vec4 quaternion(float ang, vec3 axis) {
	vec3 d = normalize(axis) * sinf(ang / 2);
	return vec4(d.x, d.y, d.z, cosf(ang / 2));
}
vec3 Rotate(vec3 u, vec4 q) {
	vec4 qinv(-q.x, -q.y, -q.z, q.w);
	vec4 qr = qmul(qmul(q, vec4(u.x, u.y, u.z, 0)), qinv);
	return vec3(qr.x, qr.y, qr.z);
}
*/
