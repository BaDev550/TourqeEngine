#include "GraphicsSubsystem.h"
#include <Engine/Core/Engine.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/Window/WindowSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>

#include <Runtime/Definitions/Profiler.h>

#include <queue>

#include <imgui.h>

namespace Tourqe::Engine {
	Result GraphicsSubsystem::OnCreate(Engine* engine) {
		Subsystem::OnCreate(engine);

		auto window = engine->GetSubsystem<WindowSubsystem>()->GetWindow();
		if (!window) { return Result("Failed to find window"); }

		BEGIN_SCOPE(RHIInitilization)

		RHI::ContextDesc contextDesc{};
#ifdef _DEBUG
		contextDesc.enableValidationLayer = true;
#endif
		contextDesc.features.dynamicRendering = true;
		contextDesc.features.bufferDeviceAddress = true;
		contextDesc.features.descriptorIndexing = true;
		contextDesc.features.robustBufferAccess = true;
		contextDesc.features.runtimeDescriptorArray = true;
		contextDesc.features.synchronization2 = true;
		contextDesc.features.timelineSemaphore = true;
		contextDesc.features.samplerAnisotropy = true;
		contextDesc.platformDesc.windowHandle = window->GetHandle();
		contextDesc.platformDesc.instance = window->GetInstance();
		contextDesc.platformDesc.platform = window->GetPlatform();
		_context = RHI::CreateContext(contextDesc);
		if (!_context) 
			return Result("Failed to create context");
		
		RHI::SwapchainDesc swapchainDesc{};
		swapchainDesc.width = window->GetWidth();
		swapchainDesc.height = window->GetHeight();
		swapchainDesc.vsync = true;
		_swapchain = _context->CreateSwapchain(swapchainDesc);
		if (!_swapchain) 
			return Result("Failed to create swapchain");

		_imguiContext = _context->CreateImGuiContext();
		if (!_imguiContext) 
			return Result("Failed to create imgui context");

		{
			for (auto& fd : _frames) {
				RHI::CommandPoolDesc cmdPoolDesc{};
				cmdPoolDesc.queueFamilyIndex = _context->GetGraphicsQueue()->GetQueueFamilyIndex();
				cmdPoolDesc.usage = RHI::CommandPoolUsage::Resettable;
				fd.commandPool = _context->CreateCommandPool(cmdPoolDesc);

				RHI::CommandBufferDesc cmdBufferDesc{};
				cmdBufferDesc.type = RHI::CommandBufferType::Primary;
				fd.commandBuffer = fd.commandPool->AllocateCommandBuffer(cmdBufferDesc);

				RHI::FenceDesc fenceDesc{};
				fenceDesc.type = RHI::SemaphoreType::Binary;
				fd.imageAvailableFence = _context->CreateFence(fenceDesc);
			}
		}

		{
			RHI::FenceDesc fenceDesc{};
			fenceDesc.initialValue = MaxFramesInFlight;
			fenceDesc.type = RHI::SemaphoreType::Timeline;
			_timelineFence = _context->CreateFence(fenceDesc);
		}

		{
			_renderFinishedFences.reserve(_swapchain->GetImageCount());
			for (usize i = 0; i < _swapchain->GetImageCount(); i++) {
				RHI::FenceDesc fenceDesc{};
				fenceDesc.type = RHI::SemaphoreType::Binary;
				_renderFinishedFences.push_back(_context->CreateFence(fenceDesc));
			}
		}

		{
			RHI::ImageDesc imageDesc{};
			imageDesc.width = 1;
			imageDesc.height = 1;
			imageDesc.format = RHI::Format::RGBA8;
			imageDesc.usage = RHI::ImageUsage::Sampled | RHI::ImageUsage::TransferDst;
			imageDesc.view.type = RHI::ImageViewType::Image2D;
			imageDesc.tileMode = RHI::ImageTileMode::Optimal;
			imageDesc.sampler = RHI::StaticSampler::NearestClamp;
			u32 whiteImageData = COLOR_WHITE;

			_defaultWhiteImage = _context->CreateImage(imageDesc);
			
			Submit([=](RHI::CommandBuffer* cmd) {
				RHI::Buffer* stagingBuffer = CreateStagingBuffer(sizeof(u32));
				stagingBuffer->Write(&whiteImageData);

				cmd->TransitionImageLayout(_defaultWhiteImage, RHI::ImageLayout::TransferDst);
				cmd->CopyBufferToImage(stagingBuffer, _defaultWhiteImage, RHI::ImageLayout::TransferDst, imageDesc.width, imageDesc.height);
				cmd->TransitionImageLayout(_defaultWhiteImage, RHI::ImageLayout::ShaderReadOnly);
				});

			_context->SetFallbackImage(_defaultWhiteImage);
		}

		END_SCOPE(RHIInitilization)

		return Result();
	}

	void GraphicsSubsystem::OnUpdate() {
		RHI::CommandBuffer* cmd = BeginFrame();
		if (!cmd) return;

		{
			auto* backbuffer = GetSwapchain()->GetColorAttachments()[_imageIndex];
			auto* depthbuffer = GetSwapchain()->GetDepthAttachment();
			glm::vec2 extent = GetSwapchain()->GetExtent();

			cmd->TransitionImageLayout(backbuffer, RHI::ImageLayout::ColorAttachment);
			cmd->TransitionImageLayout(depthbuffer, RHI::ImageLayout::DepthAttachment);

			RHI::RenderingSubmitInfo submit{};
			submit.colorAttachments = { backbuffer };
			submit.depthAttachment = depthbuffer;
			submit.extent = extent;
			cmd->BeginDynamicRendering(submit);

			cmd->EndDynamicRendering();
			cmd->TransitionImageLayout(backbuffer, RHI::ImageLayout::PresentSrc);
		}

		for (auto& command : _commandQueue) { command(cmd); }
		_commandQueue.clear();

		EndFrame();
	}

	void GraphicsSubsystem::OnDestroy() {
		Subsystem::OnDestroy();

		_context->WaitDeviceIdle();

		Mem::Allocator::Destroy<RHI::Image>(_defaultWhiteImage);

		for (auto& fence : _renderFinishedFences) { Mem::Allocator::Destroy<RHI::Fence>(fence); }
		for (auto& frame : _frames) {
			Mem::Allocator::Destroy<RHI::CommandBuffer>(frame.commandBuffer);
			Mem::Allocator::Destroy<RHI::CommandPool>(frame.commandPool);
			Mem::Allocator::Destroy<RHI::Fence>(frame.imageAvailableFence);
		}

		Mem::Allocator::Destroy<RHI::Fence>(_timelineFence);
		Mem::Allocator::Destroy<RHI::Swapchain>(_swapchain);
		Mem::Allocator::Destroy(_imguiContext);
		_context->Shutdown();
		Mem::Allocator::Destroy<RHI::Context>(_context);
	}

	void GraphicsSubsystem::GetDependencyGraph(DependencyGraph & graph) {
		graph.Require<AssetSubsystem>(DependencyOrder::Before);
		graph.Require<WindowSubsystem>(DependencyOrder::After);
	}

	RHI::CommandBuffer* GraphicsSubsystem::BeginFrame() {
		auto* window = _engine->GetSubsystem<WindowSubsystem>()->GetWindow();
		if (window->IsMinimized()) {
			_canSubmitCommand = false;
			_commandQueue.clear();
			return nullptr;
		}

		if (_recreateSwapchainNextFrame) {
			_context->WaitDeviceIdle();
			_swapchain->Destroy();
			_swapchain->Recreate(window->GetWidth(), window->GetHeight());
			_recreateSwapchainNextFrame = false;
		}

		_currentFrameIndex = _signalValue % MaxFramesInFlight;
		const u64 waitValue = _nextSignalValue - MaxFramesInFlight;
		FrameData& frame = _frames[_currentFrameIndex];

		_timelineFence->Wait(_nextSignalValue - MaxFramesInFlight);
	
		Result result = _swapchain->AcquireNextImage(_imageIndex, frame.imageAvailableFence);
		if (result.GetMessage() == "SwapchainIsOutOfDate") {
			_recreateSwapchainNextFrame = true;
			return BeginFrame();
		}
		else if (result.GetMessage() == "FailedToAcquireSwapchainImage" || result.GetMessage() == "SwapchainIsSuboptimal") {
			_recreateSwapchainNextFrame = true;
		}

		for (auto& fo : frame.trackedObjects) { Mem::Allocator::Destroy(fo); }
		frame.trackedObjects.clear();

		frame.commandPool->Reset();
		frame.commandBuffer->Begin();
		_frameRecording = true;
		_canSubmitCommand = true;

		return frame.commandBuffer;
	}

	void GraphicsSubsystem::EndFrame() {
		if (!_frameRecording) return;

		FrameData* frame = GetCurrentFrameData();
		RHI::Queue* graphicsQueue = _context->GetGraphicsQueue();

		frame->commandBuffer->End();

		_timelineFence->Signal(_nextSignalValue++);
		graphicsQueue->Submit(
			frame->commandBuffer, 
			{ frame->imageAvailableFence }, 
			{ _renderFinishedFences[_imageIndex], _timelineFence }
		);

		_signalValue++;

		graphicsQueue->Present(_swapchain, _imageIndex, { _renderFinishedFences[_imageIndex] });
		_frameRecording = false;
	}

	void GraphicsSubsystem::Submit(CommandFunc func) { 
		if (!_canSubmitCommand) return;
		_commandQueue.push_back(func); 
	}

	void GraphicsSubsystem::Track(RHI::Object* object) { GetCurrentFrameData()->trackedObjects.push_back(object); }

	RHI::Buffer* GraphicsSubsystem::CreateStagingBuffer(usize dataSizeInBytes)
	{
		RHI::BufferDesc desc{};
		desc.usage = RHI::BufferUsage::TransferSrc;
		desc.memoryUsage = RHI::MemoryUsage::CPUToGPU;
		desc.allocationFlags = RHI::AllocationFlagBits::HostAccessSequentialWrite | RHI::AllocationFlagBits::CreateMapped;
		desc.size = dataSizeInBytes;
		RHI::Buffer* stagingBuffer = _context->CreateBuffer(desc);
		stagingBuffer->SetObjectDebugName("RHIObjectStagingBuffer");
		
		Track(stagingBuffer);
		return stagingBuffer;
	}

	GraphicsSubsystem::FrameData* GraphicsSubsystem::GetCurrentFrameData() {
		Logger::Assert(_frameRecording, "GraphicsSubsystem", "No active frame");
		return &_frames[_currentFrameIndex];
	}
}