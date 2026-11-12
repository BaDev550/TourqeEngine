#include "OccluderPass.h"
#include <Runtime/Definitions/Types.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>
#include <Runtime/Definitions/Profiler.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tourqe::Engine {
	void OccluderPass::OnInitialize() {
		auto assetSystem = GetEngine()->GetSubsystem<AssetSubsystem>();
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		auto* context = renderSystem->GetContext();
		{
			Runtime::RHI::ImageDesc occluderDesc{};
			occluderDesc.format = Runtime::RHI::Format::RGBA8_UNORM;
			occluderDesc.width = LightMapResolution;
			occluderDesc.height = LightMapResolution;
			occluderDesc.sampler = Runtime::RHI::StaticSampler::LinearClamp;
			occluderDesc.tileMode = Runtime::RHI::ImageTileMode::Optimal;
			occluderDesc.view.type = Runtime::RHI::ImageViewType::Image2D;
			occluderDesc.usage = Runtime::RHI::ImageUsage::ColorAttachment | Runtime::RHI::ImageUsage::Sampled;
			_occluderImage = context->CreateImage(occluderDesc);

			Shader* forwardPassVertexShader = assetSystem->Get<Shader>("2D_forward_vert.vert");
			Shader* forwardPassFragmentShader = assetSystem->Get<Shader>("2D_forward_frag.frag");

			Runtime::RHI::PushConstantRange pcRange{};
			pcRange.size = sizeof(SpritePushConstant);
			pcRange.offset = 0;

			Runtime::RHI::PipelineDesc pipelineDesc{};
			pipelineDesc.vertexShader = &forwardPassVertexShader->GetDesc();
			pipelineDesc.fragmentShader = &forwardPassFragmentShader->GetDesc();
			pipelineDesc.colorFormats = { Runtime::RHI::Format::RGBA8_UNORM };
			pipelineDesc.depthFormat = Runtime::RHI::Format::Undefined;
			pipelineDesc.pushConstantRanges = { pcRange };
			pipelineDesc.blending = false;
			pipelineDesc.depthTest = false;
			_occluderPipeline = context->CreatePipeline(pipelineDesc);

			Runtime::RHI::BufferDesc sbDesc{};
			sbDesc.size = sizeof(SpriteData) * MaxSprites;
			sbDesc.usage = Runtime::RHI::BufferUsage::Storage | Runtime::RHI::BufferUsage::DeviceAddress;
			sbDesc.memoryUsage = Runtime::RHI::MemoryUsage::Auto;
			sbDesc.allocationFlags = Runtime::RHI::AllocationFlagBits::HostAccessRandom | Runtime::RHI::AllocationFlagBits::CreateMapped;
			_spriteDataStorageBuffer = renderSystem->GetContext()->CreateBuffer(sbDesc);
		}

		{
			Runtime::RHI::ImageDesc shadowDesc{};
			shadowDesc.format = Runtime::RHI::Format::R16_UNORM;
			shadowDesc.width = LightMapResolution;
			shadowDesc.height = MaxLights;
			shadowDesc.sampler = Runtime::RHI::StaticSampler::LinearClamp;
			shadowDesc.tileMode = Runtime::RHI::ImageTileMode::Optimal;
			shadowDesc.view.type = Runtime::RHI::ImageViewType::Image2D;
			shadowDesc.usage = Runtime::RHI::ImageUsage::ColorAttachment | Runtime::RHI::ImageUsage::Sampled;
			_shadowMapAtlas = context->CreateImage(shadowDesc);

			Shader* shadowVert = assetSystem->Get<Shader>("shadow_atlas_vert.vert");
			Shader* shadowFrag = assetSystem->Get<Shader>("shadow_atlas_frag.frag");

			Runtime::RHI::PushConstantRange shadowPcRange{};
			shadowPcRange.size = sizeof(ShadowMapPushConstant);
			shadowPcRange.offset = 0;

			Runtime::RHI::PipelineDesc pipelineDesc{};
			pipelineDesc.vertexShader = &shadowVert->GetDesc();
			pipelineDesc.fragmentShader = &shadowFrag->GetDesc();
			pipelineDesc.colorFormats = { Runtime::RHI::Format::R16_UNORM };
			pipelineDesc.depthFormat = Runtime::RHI::Format::Undefined;
			pipelineDesc.pushConstantRanges = { shadowPcRange };
			pipelineDesc.blending = false;
			pipelineDesc.depthTest = false;
			_shadowmapPipeline = context->CreatePipeline(pipelineDesc);
		}
	}

	void OccluderPass::OnDestroy() {
		Runtime::Mem::Allocator::Destroy(_spriteDataStorageBuffer);
		Runtime::Mem::Allocator::Destroy(_occluderPipeline);
		Runtime::Mem::Allocator::Destroy(_shadowmapPipeline);
		Runtime::Mem::Allocator::Destroy(_occluderImage);
		Runtime::Mem::Allocator::Destroy(_shadowMapAtlas);
	}

	void OccluderPass::Execute(const RenderFrame& frame) {
		auto* renderSystem = GetEngine()->GetSubsystem<GraphicsSubsystem>();
		auto bindlessSet = renderSystem->GetContext()->GetBindlessDescriptorSet();
		auto globalSet = renderSystem->GetContext()->GetGlobalDescriptorSet();
		if (_lightCount == 0 || _lightBDA == 0) return;

		{
			BEGIN_PROFILE_SCOPE("OccluderPass");

			frame.cmd->TransitionImageLayout(_occluderImage, Runtime::RHI::ImageLayout::ColorAttachment);

			Runtime::RHI::ColorAttachment occluderAttachment{};
			occluderAttachment.image = _occluderImage;
			occluderAttachment.loadOp = Runtime::RHI::ImageLoadOp::Clear;
			occluderAttachment.clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

			Runtime::RHI::RenderingSubmitInfo submit{};
			submit.colorAttachments = { occluderAttachment };
			submit.extent = { LightMapResolution, LightMapResolution };

			Runtime::RHI::PassDebugInfo debug{};
			debug.label = "OccluderMap";
			debug.color = glm::vec3(0.2f, 0.2f, 0.2f);

			frame.cmd->BeginDynamicRendering(submit, &debug);
			frame.cmd->BindPipeline(_occluderPipeline);
			frame.cmd->BindDescriptorSet(_occluderPipeline, bindlessSet, 0, 1);
			frame.cmd->BindDescriptorSet(_occluderPipeline, globalSet, 0, 0);

			auto spriteQueue = frame.GetDrawQueue(RenderQueue::Sprite);
			_spriteDatas.clear();

			if (u32 size = spriteQueue.size(); size != 0) {
				for (usize i = 0; i < size; i++) {
					const RenderItem& item = spriteQueue[i];
					if (!item.castShadow) continue;

					SpriteData spriteData{};
					spriteData.world = item.world;
					spriteData.vertexBufferID = item.vertexBufferIndex;
					spriteData.firstVertex = item.firstVertex;
					spriteData.frameDataIndex = frame.frameDataBufferIndex;
					spriteData.diffuseTextureID = item.textureID;
					spriteData.samplerID = item.samplerID;
					_spriteDatas.push_back(spriteData);
				}

				if (!_spriteDatas.empty()) {
					_spriteDataStorageBuffer->Write(_spriteDatas.data(), sizeof(SpriteData) * _spriteDatas.size());

					SpritePushConstant pc{};
					pc.spriteDataBDA = _spriteDataStorageBuffer->GetGPUAdress();
					frame.cmd->PushConstant(_occluderPipeline, &pc, sizeof(SpritePushConstant));
					frame.cmd->DrawVertex(6, (u32)_spriteDatas.size());
				}
			}
			frame.cmd->EndDynamicRendering();
			frame.cmd->TransitionImageLayout(_occluderImage, Runtime::RHI::ImageLayout::ShaderReadOnly);
		}
		{
			BEGIN_PROFILE_SCOPE("ShadowPass");

			frame.cmd->TransitionImageLayout(_shadowMapAtlas, Runtime::RHI::ImageLayout::ColorAttachment);

			Runtime::RHI::ColorAttachment shadowAttachment{};
			shadowAttachment.image = _shadowMapAtlas;
			shadowAttachment.loadOp = Runtime::RHI::ImageLoadOp::Clear;
			shadowAttachment.clearColor = glm::vec4(1.0f);

			Runtime::RHI::RenderingSubmitInfo shadowInfo{};
			shadowInfo.colorAttachments = { shadowAttachment };
			shadowInfo.extent = { LightMapResolution, MaxLights };

			Runtime::RHI::PassDebugInfo debugB{ "ShadowMap1D", glm::vec3(0.8f, 0.8f, 0.2f) };
			frame.cmd->BeginDynamicRendering(shadowInfo, &debugB);
			frame.cmd->BindPipeline(_shadowmapPipeline);
			frame.cmd->BindDescriptorSet(_shadowmapPipeline, bindlessSet, 0, 1);
			frame.cmd->BindDescriptorSet(_shadowmapPipeline, globalSet, 0, 0);

			ShadowMapPushConstant shadowPC{};
			shadowPC.lightBufferBDA = _lightBDA;
			shadowPC.occluderTextureID = _occluderImage->GetBindlessIndex();
			shadowPC.samplerID = _occluderImage->GetSamplerIndex();
			shadowPC.frameDataBufferID = frame.frameDataBufferIndex;
			shadowPC.resolution = static_cast<f32>(LightMapResolution);
			shadowPC.lightCount = _lightCount;
			shadowPC.maxLights = MaxLights;

			frame.cmd->PushConstant(_shadowmapPipeline, &shadowPC, sizeof(ShadowMapPushConstant), 0);
			frame.cmd->DrawVertex(6, 1);
			frame.cmd->EndDynamicRendering();

			frame.cmd->TransitionImageLayout(_shadowMapAtlas, Runtime::RHI::ImageLayout::ShaderReadOnly);
		}
	}

	void OccluderPass::SetLightData(u64 lightBDA, u32 lightCount) {
		_lightBDA = lightBDA;
		_lightCount = lightCount;
	}
}