#pragma once

#include <Runtime/Math/Vector2.h>
#include <Runtime/Math/Vector3.h>

namespace Tourqe::Engine {
	struct SpriteVertex {
		Runtime::Math::Vec3 pos;
		Runtime::Math::Vec3 norm;
		Runtime::Math::Vec2 uv;
	};
}