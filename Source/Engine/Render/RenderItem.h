#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Buffer/RHIBuffer.h>
#include <Runtime/Math/Mat4.h>

namespace Tourqe::Engine {
	struct RenderItem {
		Runtime::Math::Mat4 world = Runtime::Math::Mat4();

		b8 castShadow = true;
		u32 drawOrder = 0;
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