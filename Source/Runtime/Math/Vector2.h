#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>

namespace Runtime::Math {
	TSTRUCT();
	struct Vec2 {
		f32 x;
		f32 y;

		Vec2() : x(0.0f), y(0.0f) {};
		Vec2(f32 X, f32 Y) : x(X), y(Y) {}
		Vec2(f32 value) : x(value), y(value) {}

		Vec2 operator+(const Vec2& v) const { return { x + v.x, y + v.y }; }
		Vec2 operator-(const Vec2& v) const { return { x - v.x, y - v.y }; }
		Vec2 operator*(const Vec2& v) const { return { x * v.x, y * v.y }; }
		Vec2 operator*(const f32& v) const { return { x * v, y * v }; }
		Vec2 operator/(const Vec2& v) const { return { x / v.x, y / v.y }; }

		[[nodiscard]] const f32 Dot(const Vec2& v2) const { return ((x * v2.x) + (y * v2.y)); }
		[[nodiscard]] const f32 Length() const { return std::sqrt((x * x) + (y * y)); }
		[[nodiscard]] const Vec2 Normalize(const Vec2& v) const {
			f32 len = Length();
			if (len > 0.0f) return Vec2(v.x / len, v.y / len);
		}

		const std::string Str() const { return std::format("x:{} y:{}", x, y); }
	};

}