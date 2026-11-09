#pragma once

#include <Engine/Asset/AssetStreamer.h>
#include <Engine/Asset/Texture/Texture2D.h>

namespace Tourqe::Engine {
    TCLASS();
	class ENGINE_API Texture2DStreamer final : public AssetStreamer {
        GENERATE_CLASS(Texture2DStreamer);
	public:
		virtual Runtime::Result Cook(AssetSource& source) override;
        virtual Asset* Import(AssetSource& source) override;

        virtual std::string GetAssetClassName() override { return Texture2D::GetStaticClassName().data(); };
	};
}