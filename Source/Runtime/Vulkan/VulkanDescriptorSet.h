#pragma once

#include <Runtime/RHI/Descriptor/RHIDescriptorSet.h>
#include <vulkan/vulkan.h>

namespace Runtime::RHI {
	class VulkanContext;

	class VulkanDescriptorSet final : public DescriptorSet {
	public:
		VulkanDescriptorSet(VulkanContext* context);
		~VulkanDescriptorSet();

		virtual void WriteImage(u32 binding, u32 arrayIndex, DescriptorType type, Image* image) final;
		virtual void WriteBuffer(u32 binding, u32 arrayIndex, DescriptorType type, Buffer* buffer) final;

		[[nodiscard]] VkDescriptorSet& GetDescriptorSet() { return _set; }
		[[nodiscard]] VkDescriptorSetLayout& GetOwningLayout() { return _ownerLayout; }
	private:
		VkDescriptorSet _set;
		VkDescriptorPool _ownerPool;
		VkDescriptorSetLayout _ownerLayout;

		b8 _freeItSelf = true;

		friend class VulkanDescriptorHeap;
	};
}