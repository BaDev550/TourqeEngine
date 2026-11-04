#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>
#include <math.h>

namespace Runtime::Math {
	TCLASS();
	struct Vector3 {
		f32 x;
		f32 y;
		f32 z;

		Vector3() : x(0.0f), y(0.0f), z(0.0f) { };
		Vector3(f32 X, f32 Y, f32 Z) : x(X), y(Y), z(Z) { }
		Vector3(f32 value) : x(value), y(value), z(value) { }

		__forceinline Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
		__forceinline Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
		__forceinline Vector3 operator*(const Vector3& v) const { return { x * v.x, y * v.y, z * v.z }; }
		__forceinline Vector3 operator/(const Vector3& v) const { return { x / v.x, y / v.y, z / v.z }; }
	};

	inline f64 Dot(const Vector3& v1, const Vector3& v2) { return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)); }
	inline f64 Length(const Vector3& v) { return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)); }
	inline Vector3 Normalize(const Vector3& v) {
		f64 l = Length(v);
		return { v.x / l, v.y / l, v.z / l };
	}
}