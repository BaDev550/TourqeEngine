#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Runtime/Definitions/Types.h>

namespace Tourqe::Engine {
	TCLASS();
	class PointLightComponent final : public BaseComponent {
		GENERATE_CLASS(PointLightComponent)
	public:
		TPROP(Edit, Color);
		Runtime::Math::Vec3 color = Runtime::Math::Vec3(1.0f);

		TPROP(Edit);
		f32 intensity = 1.0f;

		TPROP(Edit);
		f32 radius = 5.0f;
	};
}