#pragma once
#include <Engine/Subsystem/Subsystem.h>

#include <Runtime/RHI/Swapchain/RHISwapchain.h>
#include <Runtime/RHI/Context/RHIContext.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <Runtime/RHI/Buffer/RHIBuffer.h>
#include <Runtime/RHI/Pipeline/RHIPipeline.h>
#include <Runtime/RHI/ImGui/ImGuiContext.h>
#include <Runtime/RHI/Command/RHICommandPool.h>
#include <Runtime/RHI/Command/RHICommandBuffer.h>
#include <Runtime/RHI/Sync/RHIFence.h>
#include <Runtime/RHI/Sync/RHIFenceDesc.h>
#include <Runtime/RHI/Queue/RHIQueue.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorHeap.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorSet.h>
#include <Runtime/RHI/Command/RHICommandBufferDesc.h>
#include <Runtime/RHI/Command/RHICommandPoolDesc.h>

namespace Tourqe::Engine {
	using namespace Runtime;
	using CommandFunc = std::function<void(RHI::CommandBuffer* cmd)>;

	class ENGINE_API GraphicsSubsystem final : public Subsystem {
	public:
		constexpr static u32 MaxFramesInFlight = 2;
		
		virtual Result OnCreate(Engine* engine) override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
		virtual void GetDependencyGraph(DependencyGraph& graph) override;

		RHI::CommandBuffer* BeginFrame();
		void EndFrame();
		void Submit(CommandFunc func);
		void Track(RHI::Object* object);

		void BeginSwapchainPass();
		void EndSwapchainPass();

		b8 CanSubmitRenderCommands() const { return _canSubmitCommand; }

		RHI::Buffer* CreateStagingBuffer(usize dataSizeInBytes);

		[[nodiscard]] const u32 GetImageIndex() const { return _imageIndex; }
		[[nodiscard]] const u32 GetFrameIndex() const { return _currentFrameIndex; }
		[[nodiscard]] RHI::Image* GetDefaultWhiteImage() const { return _defaultWhiteImage; }
		[[nodiscard]] RHI::Context* GetContext() { return _context; }
		[[nodiscard]] RHI::Swapchain* GetSwapchain() { return _swapchain; }
		[[nodiscard]] RHI::ImGuiContext* GetImGuiContext() { return _imguiContext; }
	private:
		RHI::Context* _context = nullptr;
		RHI::Swapchain* _swapchain = nullptr;
		RHI::ImGuiContext* _imguiContext = nullptr;

		struct FrameData {
			RHI::CommandPool* commandPool = nullptr;
			RHI::CommandBuffer* commandBuffer = nullptr;
			RHI::Fence* imageAvailableFence = nullptr;

			std::vector<RHI::Object*> trackedObjects;
		} _frames[MaxFramesInFlight];

		std::vector<CommandFunc> _commandQueue;
		std::vector<RHI::Fence*> _renderFinishedFences;
		RHI::Fence* _timelineFence = nullptr;

		b8 _frameRecording = false;
		b8 _canSubmitCommand = true;
		u32 _imageIndex = 0;
		u32 _currentFrameIndex = 0;
		u64 _signalValue = 0;
		u64 _nextSignalValue = (MaxFramesInFlight + 1);

		b8 _recreateSwapchainNextFrame = false;

		RHI::Image* _defaultWhiteImage = nullptr;
	public:
		[[nodiscard]] FrameData* GetCurrentFrameData();
	};
}