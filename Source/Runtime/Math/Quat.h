#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>
#include <Runtime/Math/Vector3.h>

namespace Runtime::Math {
	TSTRUCT();
	struct Quat {
		float w;
		float x;
		float y;
		float z;

		Quat() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {};
		Quat(float W, float X, float Y, float Z) : w(W), x(X), y(Y), z(Z) {}
		Quat(const Vec3& erot) {
			Quat r = FromEuler(erot);
			w = r.w;
			x = r.x;
			y = r.y;
			z = r.z;
		}

		Quat operator+(const Quat& q) const { return { w + q.w, x + q.x, y + q.y, z + q.z }; }
		Quat operator-(const Quat& q) const { return { w - q.w, x - q.x, y - q.y, z - q.z }; }
		Quat operator*(const Quat& q) const { return { 
			w * q.w - x * q.x - y * q.y - z * q.z, 
			w * q.x + x * q.w + y * q.z - z * q.y, 
			w * q.y - x * q.z + y * q.w + z * q.x, 
			w * q.z + x * q.y - y * q.x + z * q.w };
		}
		Quat operator*(const f32& v) const { return Quat(w * v, x * v, y * v, z * v); }

		[[nodiscard]] const f32 Length() const { return std::sqrt((w * w) + (x * x) + (y * y) + (z * z)); }
		[[nodiscard]] const Quat Conjugate() const { return Quat(w, -x, -y, -z); }
		[[nodiscard]] const Quat Normalize() const {
			f32 len = Length();
			if (len > 0.0f) { return Quat(w / len, x / len, y / len, z / len); }
		}

		[[nodiscard]] const Vec3 ToEuler() const {
			Vec3 res;
			f64 sinrCosp = 2.0f * (w * x + y * z);
			f64 cosrCosp = 1.0f - (2.0f * ((x * x) + (y * y)));
			f64 sinp = 2.0f * (w * y - z * x);
			res.x = std::atan2(sinrCosp, cosrCosp);
			if (std::abs(sinp) >= 1.0f) {
				res.y = std::copysign(PI / 2, sinp);
			}
			else {
				res.y = std::asin(sinp);
			}
			f64 sinyCosp = 2.0f * (w * z + x * y);
			f64 cosyCosp = 1.0f - (2.0f * ((y * y) + (z * z)));
			res.z = std::atan2(sinyCosp, cosyCosp);
			return res;
		}

		[[nodiscard]] const Quat FromEuler(const Math::Vec3& eRot) const {
			f64 cy = std::cos(eRot.z * 0.5f);
			f64 sy = std::sin(eRot.z * 0.5f);
			f64 cp = std::cos(eRot.y * 0.5f);
			f64 sp = std::sin(eRot.y * 0.5f);
			f64 cr = std::cos(eRot.x * 0.5f);
			f64 sr = std::sin(eRot.x * 0.5f);

			Quat q;
			q.w = cr * cp * cy + sr * sp * sy;
			q.x = sr * cp * cy - cr * sp * sy;
			q.y = cr * sp * cy + sr * cp * sy;
			q.z = cr * cp * sy - sr * sp * cy;
			return q;
		}

		const std::string Str() const { return std::format("x:{} y:{} z:{} w:{}", x, y, z, w); }
	};
}