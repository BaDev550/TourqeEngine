#pragma once

#include <Runtime/Definitions/Types.h>
#include <Engine/Renderer/RenderGraph/RenderPassResourceContainer.h>
#include <Engine/Renderer/RenderGraph/RenderPass.h>

namespace Tourqe::Engine {
	using RenderPassAccess = std::pair<RenderPassState, i32>;
	class RenderGraph final {
	public:
		~RenderGraph();

		void RegisterPass(RenderPass* pass);

		void Compile();
		void ClearPasses();
		void Execute(Runtime::RHI::CommandBuffer* cmd);

		RenderPassResourceHandle Write(RenderPassResource resource);
		RenderPassResourceHandle Read(RenderPassResource resource);
		RenderPassResourceHandle ReadWrite(RenderPassResource resource);

		[[nodiscard]] RHI::Image* GetImage(RenderPassResourceHandle handle);
		[[nodiscard]] RHI::Image* GetImage(const std::string& name);
		[[nodiscard]] const std::vector<RenderPassResource>& GetResources() const { return _resources; }
	private:
		std::array<std::vector<RenderPass*>, (u8)RenderPassState::Count> _passes;

		std::vector<RenderPassResource> _resources;
		std::unordered_map<std::string, i32> _resourceLookupTable;
	};
}