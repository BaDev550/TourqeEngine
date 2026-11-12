#include "VulkanImGuiContext.h"
#include "VulkanQueue.h"
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"

#include <Runtime/Memory/Allocator.h>
#include <Runtime/RHI/ImGui/ImGuiStyle.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_vulkan.h>

namespace Runtime::RHI {
	void VulkanImGuiContext::CreateContext() {
		auto* context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);

		auto& io = ImGui::GetIO();
		auto& pio = ImGui::GetPlatformIO();
		io.WantCaptureKeyboard = true;
		io.WantCaptureMouse = true;
		io.UserData = &_owner;
		io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigViewportsNoAutoMerge = false;
		io.ConfigViewportsNoTaskBarIcon = true;

		if (!pio.Platform_CreateVkSurface) {
			pio.Platform_CreateVkSurface = [](ImGuiViewport* vp, ImU64, const void*, ImU64* outSurface) -> int {
				auto* ctx = static_cast<VulkanContext*>(ImGui::GetIO().UserData);
				return static_cast<int>(ctx->CreateSurfaceForWindow(
					vp->PlatformHandleRaw, reinterpret_cast<VkSurfaceKHR*>(outSurface)));
				};
		}

		ApplyStyle(Accent::Orange);
	}

	void VulkanImGuiContext::CreateNativeImGui() {
		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		vkCreateDescriptorPool(_owner.GetDevice(), &pool_info, nullptr, &_imguiDescriptorPool); // add check

		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = _owner.GetInstance();
		init_info.ApiVersion = VK_API_VERSION_1_3;
		init_info.PhysicalDevice = _owner.GetPhysicalDevice();
		init_info.Device = _owner.GetDevice();
		init_info.Queue = _queue->GetVkQueue();
		init_info.QueueFamily = _queue->GetQueueFamilyIndex();
		init_info.DescriptorPool = _imguiDescriptorPool;
		init_info.MinImageCount = 2;
		init_info.ImageCount = 2;
		init_info.UseDynamicRendering = true;
		
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
		VkFormat viewportFormat = VK_FORMAT_R8G8B8A8_SRGB;
		VkPipelineRenderingCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
		info.colorAttachmentCount = 1;
		info.pColorAttachmentFormats = &format;
		info.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;

		VkPipelineRenderingCreateInfoKHR viewportinfo{};
		viewportinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
		viewportinfo.colorAttachmentCount = 1;
		viewportinfo.pColorAttachmentFormats = &viewportFormat;
		
		init_info.PipelineInfoMain.PipelineRenderingCreateInfo = info;
		init_info.PipelineInfoForViewports.PipelineRenderingCreateInfo = viewportinfo;
		
		ImGui_ImplVulkan_Init(&init_info);
		Logger::Info("VulkanImGuiContext", "ImGui initialized");
	}

	VulkanImGuiContext::VulkanImGuiContext(VulkanContext* owner, Runtime::RHI::Queue* queue) 
		: _owner(*owner), _queue(static_cast<VulkanQueue*>(queue)) { }

	VulkanImGuiContext::~VulkanImGuiContext() {
		ImGui_ImplVulkan_Shutdown();
		vkDestroyDescriptorPool(_owner.GetDevice(), _imguiDescriptorPool, nullptr);
		ImGui::DestroyContext();
	}

	void VulkanImGuiContext::Begin() const {
		ImGui_ImplVulkan_NewFrame();
	}

	void VulkanImGuiContext::Flush(Runtime::RHI::CommandBuffer* cmd) const {
		VulkanCommandBuffer* vkCmd = static_cast<VulkanCommandBuffer*>(cmd);
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vkCmd->GetVkCommandBuffer());
	}

	void* VulkanImGuiContext::GetNativeImGui() const { return ImGui::GetCurrentContext(); }
}