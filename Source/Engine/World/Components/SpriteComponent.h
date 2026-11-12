#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Runtime/Containers/UUID.h>

namespace Tourqe::Engine {
	TCLASS();
	class SpriteComponent : public BaseComponent {
		GENERATE_CLASS(SpriteComponent);
	public:
		TPROP(Edit, Serialize, AssetType = "Texture2D")
		AssetHandle TextureAssetHandle = 0;

		TPROP(Edit, Serialize)
		b8 Visible;

		TPROP(Edit, Serialize)
		b8 CastShadow = true;

		TPROP(Edit, Serialize, Catagory = "Render")
		int zOrder = 0;
	};

	inline u64 BuildSpriteSort(int zOrder, f32 yPos) {
		u64 depthBits = static_cast<u64>(std::clamp(static_cast<int>(yPos + 32768.0f), 0, 65535));
		u64 zOrderBits = static_cast<u64>(std::clamp(zOrder + 32768, 0, 65535));

		u64 flags = 0;
		flags |= (zOrderBits << 48);
		flags |= (depthBits << 32);
		return flags;
	}
}