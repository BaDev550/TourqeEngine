#include "ForwardRenderPass.h"
#include <Runtime/Definitions/Types.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Window/WindowSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>

namespace Tourqe::Engine {
	void ForwardRenderPass::OnInitialize() {
		auto assetSystem = GetEngine()->GetSubsystem<AssetSubsystem>();
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		{
			Shader* forwardPassVertexShader = assetSystem->Get<Shader>("base_forward_vert.vert");
			Shader* forwardPassFragmentShader = assetSystem->Get<Shader>("base_forward_frag.frag");

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
		}
	}

	void ForwardRenderPass::OnDestroy() {
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		renderSystem->GetContext()->WaitDeviceIdle();
		Runtime::Mem::Allocator::Destroy(_forwardPassPipeline);
	}

	void ForwardRenderPass::Execute(const RenderFrame& frame) {
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();

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

		for (const RenderItem& item : frame.GetDrawQueue(RenderQueue::Sprite)) {
			SpritePushConstant pc;
			pc.vertexBufferID = item.vertexBufferIndex;
			pc.firstVertex = item.firstVertex;
			pc.frameDataIndex = frame.frameDataBufferIndex;
			pc.world = item.world;
			pc.textureID = item.textureID;
			pc.samplerID = item.samplerID;
			
			frame.cmd->PushConstant(_forwardPassPipeline, &pc, sizeof(SpritePushConstant));
			frame.cmd->DrawVertex(item.vertexCount);
		}

		frame.cmd->EndDynamicRendering();
		frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::PresentSrc);
	}
}