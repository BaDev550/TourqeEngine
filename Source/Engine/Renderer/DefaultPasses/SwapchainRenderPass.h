#pragma once

#include <Engine/Renderer/RenderGraph/RenderPass.h>

namespace Tourqe::Engine {
	TCLASS();
	class ENGINE_API SwapchainRenderPass final : public RenderPass {
		GENERATE_CLASS(SwapchainRenderPass)
	public:
		virtual void RegisterResources(RenderPassResourceContainer& container) final;
		virtual void Execute(Runtime::RHI::CommandBuffer* cmd, RenderGraph& graph) final;

		[[nodiscard]] virtual RenderPassState GetRenderState() final { return RenderPassState::Default; }
	private:
		Runtime::RHI::Image* _colorAttachment = nullptr;
		Runtime::RHI::Image* _depthAttachment = nullptr;
	};
}