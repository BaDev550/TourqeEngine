#pragma once

#include <Engine/Render/RenderPass.h>
#include <Runtime/RHI/Pipeline/RHIPipeline.h>
#include <Runtime/Math/Mat4.h>

namespace Tourqe::Engine {
	TCLASS();
	class ENGINE_API ForwardRenderPass final : public RenderPass {
		GENERATE_CLASS(ForwardRenderPass)
	public:
		virtual void OnInitialize() final;
		virtual void OnDestroy() final;
		virtual void Execute(const RenderFrame& frame) final;

		virtual i32 GetOrder() const final { return 0; }
		virtual RenderStage GetStage() const final { return RenderStage::Scene; }
	private:
		static constexpr u32 MaxSprites = 1000;

		struct SpritePushConstant {
			u64 spriteDataBDA = ~0ull;
		};

		struct SpriteData {
			Runtime::Math::Mat4 world;
			u32 frameDataIndex;
			u32 firstVertex;
			u32 vertexBufferID;
			u32 textureID;
			u32 samplerID;
			u32 pad[3];
		};

		std::vector<SpriteData> _spriteDatas;
		Runtime::RHI::Buffer* _spriteDataStorageBuffer = nullptr;
		Runtime::RHI::Pipeline* _forwardPassPipeline = nullptr;
	};
}