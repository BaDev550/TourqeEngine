#include "SceneRenderer2D.h"

#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Window/WindowSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Asset/Shader/Shader.h>

#include <Engine/World/ECS/Scene.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/TransformComponent.h>

#include <imgui.h>

namespace Tourqe::Engine {
	Runtime::Result SceneRenderer2DSystem::OnCreate() {
		auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();

		_forwardRenderPass = Runtime::Mem::Allocator::Construct<ForwardRenderPass>();
		_forwardRenderPass->SetEngine(_engine);
		_forwardRenderPass->OnInitialize();

		Runtime::RHI::BufferDesc desc{};
		desc.size = sizeof(kSpriteVertices);
		desc.usage = Runtime::RHI::BufferUsage::Storage;
		desc.memoryUsage = Runtime::RHI::MemoryUsage::CPUToGPU;
		desc.allocationFlags = Runtime::RHI::AllocationFlagBits::CreateMapped | Runtime::RHI::AllocationFlagBits::HostAccessSequentialWrite;
		_spriteBuffer = renderSystem->GetContext()->CreateBuffer(desc);
		_spriteBuffer->Write(kSpriteVertices);

		{
			for (auto& buffer : _frameDataBuffers) {
				Runtime::RHI::BufferDesc storageDesc{};
				storageDesc.size = sizeof(glm::mat4);
				storageDesc.usage = Runtime::RHI::BufferUsage::Storage;
				storageDesc.memoryUsage = Runtime::RHI::MemoryUsage::CPUToGPU;
				storageDesc.allocationFlags = Runtime::RHI::AllocationFlagBits::CreateMapped | Runtime::RHI::AllocationFlagBits::HostAccessSequentialWrite;
				buffer = renderSystem->GetContext()->CreateBuffer(storageDesc);
			}
		}

		return Runtime::Result();
	}

	void SceneRenderer2DSystem::OnUpdate(Scene& scene) {
		auto submit = _engine->GetSubsystem<GraphicsSubsystem>();

		submit->Submit([this, &scene](Runtime::RHI::CommandBuffer* cmd) {
			auto assetSystem = _engine->GetSubsystem<AssetSubsystem>();
			auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();

			RenderFrame frame{};
			frame.cmd = cmd;
			frame.color = renderSystem->GetSwapchain()->GetColorAttachments()[renderSystem->GetImageIndex()];
			frame.depth = renderSystem->GetSwapchain()->GetDepthAttachment();
			frame.extent = renderSystem->GetSwapchain()->GetExtent();
			frame.frameIndex = renderSystem->GetFrameIndex();

			const f32 aspect = frame.extent.x / frame.extent.y;
			const f32 halfH = 5.0f;
			glm::mat4 viewProj = glm::orthoRH_ZO(-halfH * aspect, halfH * aspect, -halfH, halfH, -1.0f, 1.0f);
			viewProj[1][1] *= -1.0f;

			_frameDataBuffers[frame.frameIndex]->Write(&viewProj);
			frame.frameDataBufferIndex = _frameDataBuffers[frame.frameIndex]->GetBindlessIndex();

			scene.ForEach<TransformComponent, SpriteComponent>([&](EntityHandle entity, TransformComponent& transform, SpriteComponent& sprite) {
				if (!sprite.Visible) return;
				if (sprite.TextureAssetHandle == 0) return;

				Texture2D* asset = static_cast<Texture2D*>(assetSystem->TryAsset(sprite.TextureAssetHandle));
				if (!asset) return;

				RenderItem item{};
				item.textureID = asset->_image->GetBindlessIndex();
				item.samplerID = asset->_image->GetSamplerIndex();
				item.world = transform.world;
				item.firstVertex = 0;
				item.vertexCount = 6;
				item.instanceCount = 1;
				item.vertexBufferIndex = _spriteBuffer->GetBindlessIndex();
				frame.queues.Push(RenderQueue::Sprite, item);
				});
			_forwardRenderPass->Execute(frame);
			});
	}

	void SceneRenderer2DSystem::OnDestroy() {
		auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();
	}
}