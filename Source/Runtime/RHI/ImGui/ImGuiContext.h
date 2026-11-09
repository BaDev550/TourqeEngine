#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Command/RHICommandBuffer.h>

namespace Runtime::RHI {
	class ImGuiContext {
	public:
		ImGuiContext() = default;
		virtual ~ImGuiContext() = default;
		ImGuiContext(const ImGuiContext&) = delete;
		ImGuiContext& operator=(const ImGuiContext&) = delete;

		virtual void Begin() const = 0;
		virtual void CreateContext() = 0;
		virtual void CreateNativeImGui() = 0;
		virtual void* GetNativeImGui() const = 0;
		virtual void Flush(Runtime::RHI::CommandBuffer* cmd) const = 0;
	};
}