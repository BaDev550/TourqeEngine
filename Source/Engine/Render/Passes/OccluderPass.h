#pragma once

#include <Engine/Render/RenderPass.h>
#include <Engine/Render/Sprite/SpriteData.h>
#include <Engine/Render/Sprite/SpriteMaterial.h>

#include <Runtime/RHI/Pipeline/RHIPipeline.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Runtime/Math/Mat4.h>

#include <Engine/Render/LightData.h>

namespace Tourqe::Engine {
	TCLASS();
	class ENGINE_API OccluderPass final : public RenderPass {
		GENERATE_CLASS(OccluderPass)
	public:
		static constexpr u32 LightMapResolution = 512;
		static constexpr u32 MaxLights = 128;

		virtual void OnInitialize() final;
		virtual void OnDestroy() final;
		virtual void Execute(const RenderFrame& frame) final;

		u32 GetShadowMapTextureID() const { return _shadowMapAtlas->GetBindlessIndex(); }
		u32 GetShadowMapSamplerID() const { return _shadowMapAtlas->GetSamplerIndex(); }
		Runtime::RHI::Image* GetOccuderMap() { return _occluderImage; }
		Runtime::RHI::Image* GetShadowMap1D() { return _shadowMapAtlas; }
		void SetLightData(u64 lightBDA, u32 lightCount);

		virtual i32 GetOrder() const final { return 0; }
		virtual RenderStage GetStage() const final { return RenderStage::PreScene; }
	private:
		struct ShadowMapPushConstant {
			u64 lightBufferBDA;
			u32 occluderTextureID;
			u32 samplerID;
			u32 frameDataBufferID;
			f32 resolution;
			u32 lightCount;
			u32 maxLights;
		};

		u64 _lightBDA = 0;
		u32 _lightCount = 0;

		Runtime::RHI::Image* _occluderImage = nullptr;
		Runtime::RHI::Image* _shadowMapAtlas = nullptr;
		
		Runtime::RHI::Pipeline* _occluderPipeline = nullptr;
		Runtime::RHI::Pipeline* _shadowmapPipeline = nullptr;

		std::vector<SpriteData> _spriteDatas;
		Runtime::RHI::Buffer* _spriteDataStorageBuffer = nullptr;
	};
}