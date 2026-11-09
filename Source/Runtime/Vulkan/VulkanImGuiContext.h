#pragma once

#include <Runtime/RHI/ImGui/ImGuiContext.h>
#include <Runtime/PAL/Window/Window.h>
#include <vulkan/vulkan.h>

namespace Runtime::RHI {
	class VulkanContext;
	class VulkanQueue;

	class VulkanImGuiContext final : public ImGuiContext {
	public:
		VulkanImGuiContext(VulkanContext* owner, Runtime::RHI::Queue* queue);
		~VulkanImGuiContext();

		virtual void Begin() const final;
		virtual void CreateContext() final;
		virtual void CreateNativeImGui() final;
		virtual void* GetNativeImGui() const final;
		virtual void Flush(Runtime::RHI::CommandBuffer* cmd) const final;
	private:
		VulkanContext& _owner;
		VulkanQueue* _queue;
		VkDescriptorPool _imguiDescriptorPool;
	};
}