#pragma once

#include <Runtime/RHI/Object/RHIObject.h>
#include <Runtime/RHI/Image/RHIImageLayout.h>
#include <Runtime/RHI/Command/RHIRenderingSubmitInfo.h>

namespace Runtime::RHI {
	class Queue;
	class Image;
	class Buffer;
	class Pipeline;
	class DescriptorSet;

	class CommandBuffer : public Object {
	public:
		using Object::Object;
		virtual ~CommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Reset() = 0;
		
		virtual void BeginDynamicRendering(const RenderingSubmitInfo& info, PassDebugInfo* debugInfo = nullptr) = 0;
		virtual void EndDynamicRendering() = 0;

		virtual void TransitionImageLayout(Image* image, ImageLayout newLayout) = 0;
		virtual void CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer, size_t size) = 0;
		virtual void CopyBufferToImage(Buffer* buffer, Image* image, ImageLayout layout, u32 width, u32 height) = 0;

		virtual void DrawVertex(u32 count) = 0;

		virtual void BindPipeline(Pipeline* pipeline) = 0;
		virtual void PushConstant(Pipeline* pipeline, void* data, usize size, usize offset = 0) = 0;
		virtual void BindDescriptorSet(Pipeline* pipeline, DescriptorSet* descriptor, u32 binding, u32 set) = 0;
	};
}