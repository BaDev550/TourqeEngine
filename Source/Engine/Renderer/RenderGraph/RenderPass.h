#pragma once

#include <Engine/Renderer/RenderGraph/RenderGraphResource.h>
#include <Runtime/RHI/Command/RHICommandBuffer.h>
#include <Runtime/Reflection/ReflectObject.h>
#include <functional>

namespace Tourqe::Engine {
	class RenderPassResourceContainer;
	class RenderSubsystem;
	class RenderGraph;

	enum class RenderPassState : u8 {
		Default = 0,
		PrePass,
		Shadow,
		EarlyPass,
		Overlay
	};

	TCLASS();
	class RenderPass : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(RenderPass)
	public:
		RenderPass() : _alive(false) {}
		virtual ~RenderPass() = default;

		virtual void RegisterResources(RenderPassResourceContainer& container) {};
		virtual void RequestResources(RenderGraph& graph) {};
		virtual void Execute(Runtime::RHI::CommandBuffer* cmd, RenderGraph& graph) = 0;

		[[nodiscard]] virtual RenderPassState GetRenderState() = 0;

		void SetLife(b8 alive) { _alive = alive; }
		[[nodiscard]] __forceinline b8 IsAlive() const { return _alive; }
	protected:
		b8 _alive;
	};
}