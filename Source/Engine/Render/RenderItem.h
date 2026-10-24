#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Buffer/RHIBuffer.h>
#include <glm/glm.hpp>

namespace Tourqe::Engine {
	struct RenderItem {
		glm::mat4 world = glm::mat4(1.0f);

		u32 textureID = 0;
		u32 samplerID = 0;
		u32 vertexBufferIndex = 0;
		Runtime::RHI::Buffer* indexBuffer = nullptr;

		u32 firstVertex = 0;
		u32 vertexCount = 0;
		u32 firstIndex = 0;
		u32 indexCount = 0;
		u32 instanceCount = 1;
	};
}