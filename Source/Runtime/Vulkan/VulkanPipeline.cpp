#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "VulkanCommandBuffer.h"

namespace Runtime::RHI {
	VulkanPipeline::VulkanPipeline(Context* context, const PipelineDesc& desc) : Pipeline(context), _desc(desc) {}
	
	VulkanPipeline::~VulkanPipeline() {
		if (_pipeline != VK_NULL_HANDLE) vkDestroyPipeline(GetOwningRHIContext<VulkanContext>()->GetDevice(), _pipeline, nullptr);
		if (_layout != VK_NULL_HANDLE) vkDestroyPipelineLayout(GetOwningRHIContext<VulkanContext>()->GetDevice(), _layout, nullptr);
	}
}