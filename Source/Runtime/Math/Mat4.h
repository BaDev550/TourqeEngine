#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>
#include <Runtime/Math/Vector3.h>
#include <Runtime/Math/Quat.h>
#include <array>

namespace Runtime::Math {
	TSTRUCT();
	struct Mat4 {
		f32 m[16];

		Mat4() { Identity(); }

		void Identity() {
			for (int i = 0; i < 16; i++) {
				m[i] = 0.0f;
			}
			m[0] = 1.0f;
			m[5] = 1.0f;
			m[10] = 1.0f;
			m[15] = 1.0f;
		}

		Mat4 operator*(const Mat4& o) const {
			Mat4 res;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					res.m[c * 4 + r] =
						m[0 * 4 + r] * o.m[c * 4 + 0] +
						m[1 * 4 + r] * o.m[c * 4 + 1] +
						m[2 * 4 + r] * o.m[c * 4 + 2] +
						m[3 * 4 + r] * o.m[c * 4 + 3];
				}
			}
			return res;
		}

		static const Mat4 Rotate(const Quat& q) {
			Mat4 mat{};
			f32 xx = q.x * q.x;
			f32 yy = q.y * q.y;
			f32 zz = q.z * q.z;
			f32 xy = q.x * q.y;
			f32 xz = q.x * q.z;
			f32 yz = q.y * q.z;
			f32 wx = q.w * q.x;
			f32 wy = q.w * q.y;
			f32 wz = q.w * q.z;

			mat.m[0] = 1.0 - 2.0 * (yy + zz);
			mat.m[1] = 2.0 * (xy + wz);
			mat.m[2] = 2.0 * (xz - wy);
			mat.m[3] = 0.0;

			mat.m[4] = 2.0 * (xy - wz);
			mat.m[5] = 1.0 - 2.0 * (xx + zz);
			mat.m[6] = 2.0 * (yz + wx);
			mat.m[7] = 0.0;

			mat.m[8] = 2.0 * (xz + wy);
			mat.m[9] = 2.0 * (yz - wx);
			mat.m[10] = 1.0 - 2.0 * (xx + yy);
			mat.m[11] = 0.0;

			mat.m[12] = 0.0;
			mat.m[13] = 0.0;
			mat.m[14] = 0.0;
			mat.m[15] = 1.0;

			return mat;
		}

		static Mat4 Translation(const Vec3& pos) {
			Mat4 mat{};
			mat.m[12] = pos.x;
			mat.m[13] = pos.y;
			mat.m[14] = pos.z;
			return mat;
		}

		static Mat4 Scale(const Vec3& scale) {
			Mat4 mat{};
			mat.m[0] = scale.x;
			mat.m[5] = scale.y;
			mat.m[10] = scale.z;
			return mat;
		}

		const std::string Str() const { 
			std::string res = "";
			for (int r = 0; r < 4; ++r) {
				res += std::format("[ {:6.2f}, {:6.2f}, {:6.2f}, {:6.2f} ]\n", m[0 * 4 + r], m[1 * 4 + r], m[2 * 4 + r], m[3 * 4 + r]);
			}
			return res;
		}
	};
}