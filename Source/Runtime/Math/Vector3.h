#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>
#include <math.h>

namespace Runtime::Math {
	TSTRUCT();
	struct Vec3 {
		f32 x;
		f32 y;
		f32 z;

		Vec3() : x(0.0f), y(0.0f), z(0.0f) { };
		Vec3(f32 X, f32 Y, f32 Z) : x(X), y(Y), z(Z) { }
		Vec3(f32 value) : x(value), y(value), z(value) { }

		Vec3 operator+(const Vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
		Vec3 operator-(const Vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }
		Vec3 operator*(const Vec3& v) const { return { x * v.x, y * v.y, z * v.z }; }
		Vec3 operator*(const f32& v) const { return { x * v, y * v, z * v }; }
		Vec3 operator/(const Vec3& v) const { return { x / v.x, y / v.y, z / v.z }; }

		[[nodiscard]] const f32 Dot(const Vec3& v2) const { return ((x * v2.x) + (y * v2.y) + (z * v2.z)); }
		[[nodiscard]] const f32 Length() const { return sqrt((x * x) + (y * y) + (z * z)); }
		[[nodiscard]] const Vec3 Normalize() const {
			f32 len = Length();
			if (len > 0.0f) return Vec3(x / len, y / len, z / len);
		}

		const std::string Str() const { return std::format("x:{} y:{} z:{}", x, y, z); }
	};

}