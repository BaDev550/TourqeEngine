#pragma once

#include <Runtime/Math/Vector2.h>
#include <Runtime/Math/Vector4.h>

namespace Tourqe::Engine {
	struct LightData {
		Runtime::Math::Vec2 lightPos = { 0.0f, 0.0f };
		f32 lightRadius = 5.0f;
		f32 lightIntensity = 1.0f;
		Runtime::Math::Vec4 lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}