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

		[[nodiscard]] RHI::Image* GetImage(const std::string& name);
	private:
		std::vector<RenderPass*> _passes;
		std::unordered_map<RenderPassState, i32> _passesLookupTable;

		RenderPassResourceContainer _accessResourceContainer;
		std::unordered_map<std::string, RenderPassAccess> _accessResourceLinkedPasses;
	};
}