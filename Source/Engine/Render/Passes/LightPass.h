#pragma once
#include <Engine/Render/RenderPass.h>
#include <Runtime/RHI/Pipeline/RHIPipeline.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <Runtime/Math/Vector3.h>

namespace Tourqe::Engine {
	TCLASS();
	class ENGINE_API LightPass : public RenderPass { // TEMP PASS
		GENERATE_CLASS(LightPass)
	public:
		void OnInitialize() override;
		void OnDestroy() override;
		void Execute(const RenderFrame& frame) override;

		void SetResolution(f32 res) { _resolution = res; }
		void SetShadowMap(Runtime::RHI::Image* image) { _shadowMapAtlas = image; }

		void SetLightData(u64 lightBDA, u32 lightCount, Runtime::RHI::Image* shadowAtlas, f32 resolution, f32 maxLights) {
			_lightBDA = lightBDA;
			_lightCount = lightCount;
			_shadowMapAtlas = shadowAtlas;
			_resolution = resolution;
			_maxLights = maxLights;
		}
	private:
		struct LightPushConstant {
			u64 lightBufferBDA;
			u32 frameDataBufferID;
			u32 shadowMapID;
			u32 samplerID;
			f32 resolution;
			f32 maxLights;
			f32 _pad;
		};

		Runtime::RHI::Pipeline* _lightPipeline = nullptr;
		Runtime::RHI::Image* _shadowMapAtlas = nullptr;

		u64 _lightBDA = 0;
		u32 _lightCount = 0;
		f32 _resolution = 512.0f;
		f32 _maxLights = 64.0f;
	};
}