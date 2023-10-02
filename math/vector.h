#ifndef MINIRENDERER_MATH_VECTOR_H_
#define MINIRENDERER_MATH_VECTOR_H_

#include <math.h>
#include "math_utility.h"


union vector2
{
	struct {
		float x, y;
	};
	struct {
		float u, v;
	};
	float elements[2];
};

union vector3
{
	struct {
		float x, y, z;
	};
	struct {
		float u, v, w;
	};
	struct {
		float r, g, b;
	};
	float elements[3];
};

union vector4
{
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};
	float elements[4];
};

/////////////////////////////////////////////////////////////////
//
// Constant vector variables
// 
/////////////////////////////////////////////////////////////////
const vector2 VECTOR2_ZERO = vector2{ 0.0f, 0.0f };
const vector2 VECTOR2_ONE = vector2{ 1.0f, 1.0f };

const vector3 VECTOR3_ZERO = vector3{ 0.0f, 0.0f, 0.0f };
const vector3 VECTOR3_ONE = vector3{ 1.0f, 1.0f, 1.0f };

const vector4 VECTOR4_ZERO = vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
const vector4 VECTOR4_ONE = vector4{ 1.0f, 1.0f, 1.0f, 1.0f };

/////////////////////////////////////////////////////////////////
//
// Vector convert
//
/////////////////////////////////////////////////////////////////
inline vector3 vector2_to_3(const vector2 v) { return vector3{ v.x, v.y, 0.0f }; }
inline vector4 vector3_to_4(const vector3 v) { return vector4{ v.x, v.y, v.z, 0.0f }; }
inline vector3 vector4_to_3(const vector4 v) { return vector3{ v.x, v.y, v.z }; }
inline vector2 vector4_to_2(const vector4 v) { return vector2{ v.x, v.y }; }
inline vector2 vector3_to_2(const vector3 v) { return vector2{ v.x, v.y }; }

/////////////////////////////////////////////////////////////////
//
// Vector Math
// 
/////////////////////////////////////////////////////////////////
inline vector2 operator+(const vector2 a, const vector2 b) { return vector2{ a.x + b.x, a.y + b.y }; }
inline vector3 operator+(const vector3 a, const vector3 b) { return vector3{ a.x + b.x, a.y + b.y, a.z + b.z }; }
inline vector4 operator+(const vector4 a, const vector4 b) { return vector4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }

inline vector2 operator-(const vector2 a, const vector2 b) { return vector2{ a.x - b.x, a.y - b.y }; }
inline vector3 operator-(const vector3 a, const vector3 b) { return vector3{ a.x - b.x, a.y - b.y, a.z - b.z }; }
inline vector4 operator-(const vector4 a, const vector4 b) { return vector4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

inline bool operator==(const vector2 a, const vector2 b) { return a.x == b.x && a.y == b.y;  }
inline bool operator==(const vector3 a, const vector3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
inline bool operator==(const vector4 a, const vector4 b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

inline float dot(const vector2 a, const vector2 b) { return a.x * b.x + a.y * b.y; }
inline float dot(const vector3 a, const vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float dot(const vector4 a, const vector4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

///
/// \brief Vector2 cross is used to evaluate the facing dir of a triangle.
/// 
inline float cross(const vector2 a, const vector2 b) { return a.x * b.y - a.y * b.x; }
inline vector3 cross(const vector3 a, const vector3 b) { return vector3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
//vector4 cross is omitted due to the little frequency of usage.

inline vector2 operator/(const vector2 a, float w) { return vector2{ a.x / w, a.y / w }; }
inline vector3 operator/(const vector3 a, float w) { return vector3{ a.x / w, a.y / w, a.z / w }; }
inline vector4 operator/(const vector4 a, float w) { return vector4{ a.x / w, a.y / w, a.z / w, a.w / w }; }

inline vector2 operator*(const vector2 a, float s) { return vector2{ a.x * s, a.y * s }; }
inline vector3 operator*(const vector3 a, float s) { return vector3{ a.x * s, a.y * s, a.z * s }; }
inline vector4 operator*(const vector4 a, float s) { return vector4{ a.x * s, a.y * s, a.z * s, a.w * s }; }

inline float magnitude_squared(vector2 a) { return dot(a, a); }
inline float magnitude_squared(vector3 a) { return dot(a, a); }
inline float magnitude_squared(vector4 a) { return dot(a, a); }

inline float magnitude(vector2 a) { return sqrtf(magnitude_squared(a)); }
inline float magnitude(vector3 a) { return sqrtf(magnitude_squared(a)); }
inline float magnitude(vector4 a) { return sqrtf(magnitude_squared(a)); }

inline vector2 normalize(const vector2 a) {
	float square_magnitude = magnitude_squared(a);
	if (fabsf(square_magnitude - 1.0f) < SMALL_ABSOLUTE_FLOAT)
		return a;
	if (square_magnitude == 0.0f)
		return VECTOR2_ZERO;
	return a * (1.0f / sqrtf(square_magnitude));
}
inline vector3 normalize(const vector3 a) {
	float square_magnitude = magnitude_squared(a);
	if (fabsf(square_magnitude - 1.0f) < SMALL_ABSOLUTE_FLOAT)
		return a;
	if (square_magnitude == 0.0f)
		return VECTOR3_ZERO;
	return a * (1.0f / sqrtf(square_magnitude));
}
inline vector4 normalize(const vector4 a) {
	float square_magnitude = magnitude_squared(a);
	if (fabsf(square_magnitude - 1.0f) < SMALL_ABSOLUTE_FLOAT)
		return a;
	if (square_magnitude == 0.0f)
		return VECTOR4_ZERO;
	return a * (1.0f / sqrtf(square_magnitude));
}

inline vector2 lerp(const vector2 a, const vector2 b, float t) {
	return vector2{
		lerp(a.x, b.x, t),
		lerp(a.y, b.y, t), };
}
inline vector3 lerp(const vector3 a, const vector3 b, float t) {
	return vector3{
		lerp(a.x, b.x, t),
		lerp(a.y, b.y, t),
		lerp(a.z, b.z, t) };
}
inline vector4 lerp(const vector4 a, const vector4 b, float t) {
	return vector4{
		lerp(a.x, b.x, t),
		lerp(a.y, b.y, t),
		lerp(a.z, b.z, t),
		lerp(a.w, b.w, t) };
}
#endif // !MINIRENDERER_MATH_VECTOR_H_
