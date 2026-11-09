#pragma once

#include <Engine/Render/Sprite/SpriteMaterial.h>

#include <Runtime/RHI/Command/RHICommandBuffer.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <Runtime/RHI/Image/RHIImageLayout.h>
#include <mutex>

namespace Tourqe::Engine {
	struct NormalGen {
		Runtime::RHI::Image* srcDiffuse;
		Runtime::RHI::Image* dstNormal;
		u32 dstStorageBindlessIdx;
		u32 width;
		u32 height;
		float strength;
	};

	class ImageNormalMapGenerator {
	public:
		ImageNormalMapGenerator();
		~ImageNormalMapGenerator();

		void Enqueue(const NormalGen& job);
		void FlushComputeQueue(Runtime::RHI::CommandBuffer* cmd);
	private:
		struct SpriteComputePushData {
			u32 srcTexIdx;
			u32 dstImgIdx;
			u32 width;
			u32 height;
			float strength;
		};

		std::mutex _mutex;
		std::vector<NormalGen> _pendingJobs;
		Runtime::RHI::Pipeline* _sobelPipeline;
	};
}