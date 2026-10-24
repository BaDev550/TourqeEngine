#pragma once

#include <Engine/World/System.h> // TODO(0x): move this to another folder
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/Render/ForwardRenderPass.h>
#include <Engine/Graphics/GraphicsSubsystem.h>

#include <Runtime/RHI/Pipeline/RHIPipeline.h>

namespace Tourqe::Engine {
	class ENGINE_API SceneRenderer2DSystem final : public System {
	public:
		virtual Runtime::Result OnCreate() override;
		virtual void OnUpdate(Scene& scene) override;
		virtual void OnDestroy() override;
	private:
		ForwardRenderPass* _forwardRenderPass;

		struct Vertex {
			glm::vec3 pos;
			glm::vec3 norm;
			glm::vec2 uv;
		};

		Vertex kSpriteVertices[6] = {
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }, // bottom-left
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }, // bottom-right
			{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // top-right
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }, // bottom-left
			{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // top-right
			{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // top-left
		};

		Runtime::RHI::Buffer* _frameDataBuffers[GraphicsSubsystem::MaxFramesInFlight];
		Runtime::RHI::Buffer* _spriteBuffer;
	};
}