#pragma once

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
		Overlay,
		Count
	};

	TCLASS();
	class RenderPass : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(RenderPass)
	public:
		RenderPass() = default;
		virtual ~RenderPass() = default;

		virtual void RegisterResources(RenderGraph& graph) {};
		virtual void Execute(Runtime::RHI::CommandBuffer* cmd, RenderGraph& graph) = 0;

		[[nodiscard]] virtual RenderPassState GetRenderState() = 0;

		[[nodiscard]] std::vector<i32> GetActiveResources() const { return _activeResources; }
	protected:
		std::vector<i32> _activeResources;
	};
}