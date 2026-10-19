#include "VulkanDescriptorHeap.h"
#include "VulkanDescriptorSet.h"
#include "VulkanContext.h"
#include "VulkanUtils.h"

#include <Runtime/Memory/Memory.h>

namespace Runtime::RHI {
    VulkanDescriptorHeap::VulkanDescriptorHeap(VulkanContext* context, const DescriptorHeapDesc& desc) : DescriptorHeap(context), _desc(desc) { }

    VulkanDescriptorHeap::~VulkanDescriptorHeap() {
        for (auto& layout : _layouts) {
            if (layout != VK_NULL_HANDLE) vkDestroyDescriptorSetLayout(GetOwningRHIContext<VulkanContext>()->GetDevice(), layout, nullptr);
        }

        if (_pool != VK_NULL_HANDLE) vkDestroyDescriptorPool(GetOwningRHIContext<VulkanContext>()->GetDevice(), _pool, nullptr);
    }

    DescriptorSet* VulkanDescriptorHeap::AllocDescriptorSet(const DescriptorSetAllocInfo& allocInfo) {
		std::vector<VkDescriptorSetLayoutBinding> vkBindings;
		std::vector<VkDescriptorBindingFlags> vkFlags;

		for (usize i = 0; i < allocInfo.bindings.size(); i++) {
			auto& binding = allocInfo.bindings[i];

			VkDescriptorSetLayoutBinding vkBinding{};
			vkBinding.binding = i;
			vkBinding.descriptorCount = binding.count;
			vkBinding.descriptorType = Utils::GetVkDescriptorType(binding.type);
			vkBinding.stageFlags = Utils::GetVkShaderStage(binding.stage);

			vkBindings.push_back(std::move(vkBinding));
			vkFlags.push_back(Utils::GetVkDescriptorBindingFlags(binding.flags));
		}
		VkDescriptorSetLayout layout;
		VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{};
		bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		bindingFlagsInfo.bindingCount = static_cast<u32>(vkFlags.size());
		bindingFlagsInfo.pBindingFlags = vkFlags.data();

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = &bindingFlagsInfo;
		layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
		layoutInfo.bindingCount = static_cast<u32>(vkBindings.size());
		layoutInfo.pBindings = vkBindings.data();

		Logger::Assert((vkCreateDescriptorSetLayout(GetOwningRHIContext<VulkanContext>()->GetDevice(), &layoutInfo, nullptr, &layout) == VK_SUCCESS),
			"RHIContextVulkan", "Failed to create descriptor set layout");
		_layouts.push_back(layout);

        VulkanDescriptorSet* vkSet = Mem::Allocator::Construct<VulkanDescriptorSet>(GetOwningRHIContext<VulkanContext>());
        vkSet->_ownerPool = _pool;
        vkSet->_ownerLayout = layout;
        vkSet->_freeItSelf = ((_desc.flags & DescriptorHeapCreateFlags::FreeDescriptorSet) == DescriptorHeapCreateFlags::None);
        
		VkDescriptorSetAllocateInfo vkAllocInfo{};
		vkAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		vkAllocInfo.descriptorPool = _pool;
		vkAllocInfo.descriptorSetCount = 1;
		vkAllocInfo.pSetLayouts = &layout;

		Logger::Assert((vkAllocateDescriptorSets(GetOwningRHIContext<VulkanContext>()->GetDevice(), &vkAllocInfo, &vkSet->_set) == VK_SUCCESS), "RHIVulkanDescriptorHeap",
            "Failed to allocate descriptor set");

        _sets.push_back(vkSet);
        return vkSet;
    }
}