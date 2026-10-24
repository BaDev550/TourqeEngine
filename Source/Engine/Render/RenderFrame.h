#pragma once

#include <Runtime/RHI/Command/RHICommandBuffer.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <Runtime/RHI/Buffer/RHIBuffer.h>
#include <Engine/Render/RenderQueue.h>
#include <glm/glm.hpp>

namespace Tourqe::Engine {
	struct RenderFrame {
		Runtime::RHI::CommandBuffer* cmd = nullptr;
		Runtime::RHI::Image* color = nullptr;
		Runtime::RHI::Image* depth = nullptr;
		glm::vec2 extent = glm::vec2(0.0f);

		u32 frameIndex = 0;
		u32 frameDataBufferIndex = 0;

		RenderQueues queues;
		const std::vector<RenderItem>& GetDrawQueue(RenderQueue queue) const { return queues.Get(queue); }
	};
}