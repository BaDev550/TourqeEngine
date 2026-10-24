#include "VulkanDescriptorSet.h"
#include "VulkanContext.h"

#include "VulkanImage.h"
#include "VulkanBuffer.h"
#include "VulkanUtils.h"

namespace Runtime::RHI {
	VulkanDescriptorSet::VulkanDescriptorSet(VulkanContext* context) : DescriptorSet(context) { }

	VulkanDescriptorSet::~VulkanDescriptorSet() {
		if (_freeItSelf) vkFreeDescriptorSets(GetOwningRHIContext<VulkanContext>()->GetDevice(), _ownerPool, 1, &_set);
	}

	void VulkanDescriptorSet::WriteImage(u32 binding, u32 arrayIndex, DescriptorType type, Image* image) {
		VulkanImage* vkImage = static_cast<VulkanImage*>(image);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageView = vkImage->GetImageView();
		imageInfo.imageLayout = vkImage->GetImageLayout();

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.dstSet = _set;
		write.dstBinding = binding;
		write.dstArrayElement = arrayIndex;
		write.descriptorCount = 1;
		write.descriptorType = Utils::GetVkDescriptorType(type);
		write.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(GetOwningRHIContext<VulkanContext>()->GetDevice(), 1, &write, 0, nullptr);
	}

	void VulkanDescriptorSet::WriteBuffer(u32 binding, u32 arrayIndex, DescriptorType type, Buffer* buffer) {
		VulkanBuffer* vkBuffer = static_cast<VulkanBuffer*>(buffer);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = vkBuffer->GetVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.dstSet = _set;
		write.dstBinding = binding;
		write.dstArrayElement = arrayIndex;
		write.descriptorCount = 1;
		write.descriptorType = Utils::GetVkDescriptorType(type);
		write.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(GetOwningRHIContext<VulkanContext>()->GetDevice(), 1, &write, 0, nullptr);
	}
}