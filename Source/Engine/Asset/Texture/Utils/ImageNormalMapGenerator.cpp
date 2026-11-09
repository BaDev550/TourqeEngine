#include "ImageNormalMapGenerator.h"

#include <Engine/Core/Engine.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>

#include <Runtime/RHI/Shader/ShaderDesc.h>
#include <Runtime/RHI/Image/RHIImage.h>

namespace Tourqe::Engine {
	ImageNormalMapGenerator::ImageNormalMapGenerator() {
		auto assetSystem = Engine::Get()->GetSubsystem<AssetSubsystem>();
		auto* renderSystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();
		{
			Shader* computeShader = assetSystem->Get<Shader>("2D_normalmap_generator.comp");
			
			Runtime::RHI::PushConstantRange pcRange{};
			pcRange.size = sizeof(SpriteComputePushData);
			pcRange.offset = 0;

			Runtime::RHI::PipelineDesc vertexDesc{};
			vertexDesc.colorFormats = { renderSystem->GetSwapchain()->GetColorFormat() };
			vertexDesc.depthFormat = renderSystem->GetSwapchain()->GetDepthFormat();
			vertexDesc.computeShader = &computeShader->GetDesc();
			vertexDesc.pushConstantRanges = { pcRange };
			vertexDesc.blending = false;
			vertexDesc.depthTest = false;

			_sobelPipeline = renderSystem->GetContext()->CreatePipeline(vertexDesc);
		}
	}

	ImageNormalMapGenerator::~ImageNormalMapGenerator() {
		Runtime::Mem::Allocator::Destroy(_sobelPipeline);
	}

	void ImageNormalMapGenerator::Enqueue(const NormalGen& job) {
		std::lock_guard<std::mutex> lock(_mutex);
		_pendingJobs.push_back(job);
	}

	void ImageNormalMapGenerator::FlushComputeQueue(Runtime::RHI::CommandBuffer* cmd) {
		auto* renderSystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();

		std::vector<NormalGen> jobs;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_pendingJobs.empty()) return;
			jobs.swap(_pendingJobs);
		}

		for (const auto& job : jobs) {
			cmd->TransitionImageLayout(job.dstNormal, Runtime::RHI::ImageLayout::UnorderedAccess);
		}

		auto* bindlessSet = renderSystem->GetContext()->GetBindlessDescriptorSet();

		cmd->BindPipeline(_sobelPipeline, Runtime::RHI::PipelineBindPoint::Compute);
		cmd->BindDescriptorSet(_sobelPipeline, bindlessSet, 0, 0);
		
		for (const auto& job : jobs) {
			SpriteComputePushData push{};
			push.srcTexIdx = job.srcDiffuse->GetBindlessIndex();
			push.dstImgIdx = job.dstNormal->GetBindlessIndex();
			push.width = job.width;
			push.height = job.height;
			push.strength = job.strength;

			cmd->PushConstant(_sobelPipeline, &push, sizeof(SpriteComputePushData));
			cmd->Dispatch((job.width + 15) / 16, (job.height + 15) / 16, 1);
		}

		for (const auto& job : jobs) {
			cmd->TransitionImageLayout(job.dstNormal, Runtime::RHI::ImageLayout::ShaderReadOnly);
		}
	}
}