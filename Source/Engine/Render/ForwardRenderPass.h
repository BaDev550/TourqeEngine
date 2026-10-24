#pragma once

#include <Engine/Render/RenderPass.h>
#include <Runtime/RHI/Pipeline/RHIPipeline.h>

namespace Tourqe::Engine {
	TCLASS();
	class ForwardRenderPass final : public RenderPass {
		GENERATE_CLASS(ForwardRenderPass)
	public:
		virtual void OnInitialize() final;
		virtual void OnDestroy() final;
		virtual void Execute(const RenderFrame& frame) final;

		virtual i32 GetOrder() const final { return 0; }
		virtual RenderStage GetStage() const final { return RenderStage::Scene; }
	private:
		struct SpritePushConstant {
			glm::mat4 world;
			u32 frameDataIndex;
			u32 firstVertex;
			u32 vertexBufferID;
			u32 textureID;
			u32 samplerID;
		};

		Runtime::RHI::Pipeline* _forwardPassPipeline = nullptr;
	};
}