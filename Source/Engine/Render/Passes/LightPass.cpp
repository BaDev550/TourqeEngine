#include "LightPass.h"
#include <Runtime/Definitions/Types.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>
#include <Runtime/Definitions/Profiler.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tourqe::Engine {
	void LightPass::OnInitialize() {
		auto* assetSystem = GetEngine()->GetSubsystem<AssetSubsystem>();
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		auto* context = renderSystem->GetContext();

		Shader* lightVert = assetSystem->Get<Shader>("2D_light_vert.vert");
		Shader* lightFrag = assetSystem->Get<Shader>("2D_light_frag.frag");

		Runtime::RHI::PushConstantRange pcRange{};
		pcRange.size = sizeof(LightPushConstant);
		pcRange.offset = 0;

		Runtime::RHI::PipelineDesc pipelineDesc{};
		pipelineDesc.vertexShader = &lightVert->GetDesc();
		pipelineDesc.fragmentShader = &lightFrag->GetDesc();
		pipelineDesc.colorFormats = { renderSystem->GetSwapchain()->GetColorFormat() };
		pipelineDesc.depthFormat = Runtime::RHI::Format::Undefined;
		pipelineDesc.pushConstantRanges = { pcRange };
		pipelineDesc.blending = true;
		pipelineDesc.depthTest = false;

		_lightPipeline = context->CreatePipeline(pipelineDesc);
	}

	void LightPass::OnDestroy() {
		Runtime::Mem::Allocator::Destroy(_lightPipeline);
	}

	void LightPass::Execute(const RenderFrame& frame) {
		auto renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		auto bindlessSet = renderSystem->GetContext()->GetBindlessDescriptorSet();
		auto globalSet = renderSystem->GetContext()->GetGlobalDescriptorSet();

		{
			BEGIN_PROFILE_SCOPE("LightPass");

			Runtime::RHI::ColorAttachment attachment{};
			attachment.image = frame.color;
			attachment.loadOp = Runtime::RHI::ImageLoadOp::Load;
			attachment.clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::ColorAttachment);

			Runtime::RHI::RenderingSubmitInfo info{};
			info.colorAttachments = { attachment };
			info.extent = frame.extent;

			Runtime::RHI::PassDebugInfo debug{};
			debug.label = "LightPass";
			debug.color = glm::vec3(1.0f, 0.9f, 0.2f);

			frame.cmd->BeginDynamicRendering(info, &debug);
			frame.cmd->BindPipeline(_lightPipeline);
			frame.cmd->BindDescriptorSet(_lightPipeline, bindlessSet, 0, 1);
			frame.cmd->BindDescriptorSet(_lightPipeline, globalSet, 0, 0);

			if (_lightCount > 0 && _lightBDA != 0 && _shadowMapAtlas != nullptr) {
				frame.cmd->BindPipeline(_lightPipeline);
				frame.cmd->BindDescriptorSet(_lightPipeline, bindlessSet, 0, 1);
				frame.cmd->BindDescriptorSet(_lightPipeline, globalSet, 0, 0);

				LightPushConstant pc{};
				pc.lightBufferBDA = _lightBDA;
				pc.frameDataBufferID = frame.frameDataBufferIndex;
				pc.shadowMapID = _shadowMapAtlas->GetBindlessIndex();
				pc.samplerID = _shadowMapAtlas->GetSamplerIndex();
				pc.resolution = _resolution;
				pc.maxLights = _maxLights;

				frame.cmd->PushConstant(_lightPipeline, &pc, sizeof(LightPushConstant), 0);
				frame.cmd->DrawVertex(6, _lightCount);
			}

			frame.cmd->EndDynamicRendering();
			frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::PresentSrc);
		}
	}
}