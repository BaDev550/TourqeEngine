#pragma once

#include <Runtime/RHI/Descriptor/RHIDescriptorHeap.h>
#include <vulkan/vulkan.h>

namespace Runtime::RHI {
	class VulkanContext;

	class VulkanDescriptorHeap final : public DescriptorHeap {
	public:
		VulkanDescriptorHeap(VulkanContext* context, const DescriptorHeapDesc& desc);
		virtual ~VulkanDescriptorHeap();

		virtual DescriptorSet* AllocDescriptorSet(const DescriptorSetAllocInfo& allocInfo) final;

		[[nodiscard]] virtual std::vector<DescriptorSet*>& GetDescriptorSets() noexcept final { return _sets; }
		[[nodiscard]] virtual const DescriptorHeapDesc& GetDesc() const noexcept final { return _desc; }
	private:
		DescriptorHeapDesc _desc;

		std::vector<DescriptorSet*> _sets;
		std::vector<VkDescriptorSetLayout> _layouts;

		VkDescriptorPool _pool;

		friend class VulkanContext;
	};
}