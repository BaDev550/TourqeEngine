#include "ForwardRenderPass.h"
#include <Runtime/Definitions/Types.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>
#include <Runtime/Definitions/Profiler.h>

namespace Tourqe::Engine {
	void ForwardRenderPass::OnInitialize() {
		auto assetSystem = GetEngine()->GetSubsystem<AssetSubsystem>();
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		{
			Shader* forwardPassVertexShader = assetSystem->Get<Shader>("2D_forward_vert.vert");
			Shader* forwardPassFragmentShader = assetSystem->Get<Shader>("2D_forward_frag.frag");

			Runtime::RHI::PushConstantRange pcRange{};
			pcRange.size = sizeof(SpritePushConstant);
			pcRange.offset = 0;

			Runtime::RHI::PipelineDesc vertexDesc{};
			vertexDesc.colorFormats = { renderSystem->GetSwapchain()->GetColorFormat() };
			vertexDesc.depthFormat = renderSystem->GetSwapchain()->GetDepthFormat();
			vertexDesc.vertexShader = &forwardPassVertexShader->GetDesc();
			vertexDesc.fragmentShader = &forwardPassFragmentShader->GetDesc();
			vertexDesc.pushConstantRanges = { pcRange };
			vertexDesc.blending = false;
			vertexDesc.depthTest = false;

			_forwardPassPipeline = renderSystem->GetContext()->CreatePipeline(vertexDesc);

			Runtime::RHI::BufferDesc sbDesc{};
			sbDesc.size = sizeof(SpriteData) * MaxSprites;
			sbDesc.usage = Runtime::RHI::BufferUsage::Storage | Runtime::RHI::BufferUsage::DeviceAddress;
			sbDesc.memoryUsage = Runtime::RHI::MemoryUsage::Auto;
			sbDesc.allocationFlags = Runtime::RHI::AllocationFlagBits::HostAccessRandom | Runtime::RHI::AllocationFlagBits::CreateMapped;
			_spriteDataStorageBuffer = renderSystem->GetContext()->CreateBuffer(sbDesc);
		}
	}

	void ForwardRenderPass::OnDestroy() {
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		renderSystem->GetContext()->WaitDeviceIdle();
		Runtime::Mem::Allocator::Destroy(_spriteDataStorageBuffer);
		Runtime::Mem::Allocator::Destroy(_forwardPassPipeline);
	}

	void ForwardRenderPass::Execute(const RenderFrame& frame) {
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();

		{
			BEGIN_PROFILE_SCOPE("ForwardPass");

			Runtime::RHI::ColorAttachment attachment{};
			attachment.image = frame.color;
			attachment.loadOp = Runtime::RHI::ImageLoadOp::Load;
			attachment.clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::ColorAttachment);
			frame.cmd->TransitionImageLayout(frame.depth, Runtime::RHI::ImageLayout::DepthAttachment);

			Runtime::RHI::RenderingSubmitInfo info{};
			info.colorAttachments = { attachment };
			info.depthAttachment = frame.depth;
			info.extent = frame.extent;

			Runtime::RHI::PassDebugInfo debug{};
			debug.label = "ForwardPass";
			debug.color = glm::vec3(1.0f, 0.0f, 0.0f);

			frame.cmd->BeginDynamicRendering(info, &debug);
			frame.cmd->BindPipeline(_forwardPassPipeline);

			auto* bindlessSet = renderSystem->GetContext()->GetBindlessDescriptorSet();
			auto* globalSet = renderSystem->GetContext()->GetGlobalDescriptorSet();

			frame.cmd->BindDescriptorSet(_forwardPassPipeline, bindlessSet, 0, 1);
			frame.cmd->BindDescriptorSet(_forwardPassPipeline, globalSet, 0, 0);

			auto spriteQueue = frame.GetDrawQueue(RenderQueue::Sprite);
			if (!spriteQueue.empty()) {
				std::stable_sort(spriteQueue.begin(), spriteQueue.end(), [](const RenderItem& a, const RenderItem& b) {
					return a.drawOrder < b.drawOrder;
					});

				_spriteDatas.resize(spriteQueue.size());

				int i = 0;
				for (const RenderItem& item : spriteQueue) {
					SpriteData spriteData{};
					spriteData.world = item.world;
					spriteData.vertexBufferID = item.vertexBufferIndex;
					spriteData.firstVertex = item.firstVertex;
					spriteData.frameDataIndex = frame.frameDataBufferIndex;
					spriteData.diffuseTextureID = item.textureID;
					spriteData.normalTextureID = 0;
					spriteData.samplerID = item.samplerID;

					_spriteDatas[i] = spriteData;
					i++;
				}

				if (!_spriteDatas.empty()) {
					_spriteDataStorageBuffer->Write(_spriteDatas.data(), (sizeof(SpriteData) * _spriteDatas.size()));

					SpritePushConstant pc{};
					pc.spriteDataBDA = _spriteDataStorageBuffer->GetGPUAdress();

					frame.cmd->PushConstant(_forwardPassPipeline, &pc, sizeof(SpritePushConstant), 0);
					frame.cmd->DrawVertex(6, spriteQueue.size());
				}
			}
			frame.cmd->EndDynamicRendering();
			frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::PresentSrc);
		}
	}
}