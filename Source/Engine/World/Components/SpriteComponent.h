#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Runtime/Containers/UUID.h>

namespace Tourqe::Engine {
	TCLASS();
	class SpriteComponent : public BaseComponent {
		GENERATE_CLASS(SpriteComponent);
	public:
		TPROP()
		AssetHandle TextureAssetHandle = 0;

		TPROP()
		b8 Visible;
	};
}