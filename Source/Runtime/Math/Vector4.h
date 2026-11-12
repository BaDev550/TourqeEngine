#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>
#include <math.h>

namespace Runtime::Math {
	TSTRUCT();
	struct Vec4 {
		f32 x;
		f32 y;
		f32 z;
		f32 w;

		Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
		Vec4(f32 X, f32 Y, f32 Z, f32 W) : x(X), y(Y), z(Z), w(W) {}
		Vec4(f32 value) : x(value), y(value), z(value), w(value) {}

		Vec4 operator+(const Vec4& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
		Vec4 operator-(const Vec4& v) const { return { x - v.x, y - v.y, z - v.z, w - v.w }; }
		Vec4 operator*(const Vec4& v) const { return { x * v.x, y * v.y, z * v.z, w * v.w }; }
		Vec4 operator*(const f32& v) const { return { x * v, y * v, z * v, w * v }; }
		Vec4 operator/(const Vec4& v) const { return { x / v.x, y / v.y, z / v.z, w / v.w }; }
		Vec4 operator-() const { return { -x, -y, -z, -w }; }

		[[nodiscard]] const f32 Dot(const Vec4& v2) const { return ((x * v2.x) + (y * v2.y) + (z * v2.z) + (w * v2.w)); }
		[[nodiscard]] const f32 Length() const { return sqrt((x * x) + (y * y) + (z * z) + (w * w)); }
		[[nodiscard]] const Vec4 Normalize() const {
			f32 len = Length();
			if (len > 0.0f) return Vec4(x / len, y / len, z / len, w / len);
		}

		const std::string Str() const { return std::format("x:{} y:{} z:{} w:{}", x, y, z, w); }
	};

}