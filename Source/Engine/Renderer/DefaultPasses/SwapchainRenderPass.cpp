#include "SwapchainRenderPass.h"

#include <Engine/Renderer/RenderGraph/RenderPassResourceContainer.h>

namespace Tourqe::Engine {
	void SwapchainRenderPass::RegisterResources(RenderPassResourceContainer& container) {
		container.Write({ "SwapchainColorAttachment", _colorAttachment });
		container.Write({ "SwapchainDepthAttachment", _depthAttachment });
	}

	void SwapchainRenderPass::Execute(Runtime::RHI::CommandBuffer* cmd, RenderGraph& graph) {
		graph
	}
}