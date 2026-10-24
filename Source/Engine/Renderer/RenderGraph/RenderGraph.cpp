#include "RenderGraph.h"

#include <Engine/Core/Engine.h>
#include <Engine/Renderer/RenderSubsystem.h>

#include <Runtime/Memory/Memory.h>

namespace Tourqe::Engine {
	RenderGraph::~RenderGraph() {
		ClearPasses();
	}

	void RenderGraph::RegisterPass(RenderPass* pass) {
		if (!pass) {
			Logger::Error("RenderGraph", "Failed to initialize render graph");
			return;
		}
		u8 renderState = static_cast<u8>(pass->GetRenderState());
		u32 passIndex = static_cast<u32>(_passes[renderState].size());

		pass->RegisterResources(*this);
		for (usize i = 0; i < _accessResourceContainer.GetResources().size(); i++) {
			auto& resource = container.GetResource(i);

			_resourceLinkedPasses[resource] = { containerIndex, i };
		}
		_resourceContainers[std::move(container)] = { renderState, passIndex };

		passes.push_back(std::move(pass));
	}

	RenderPassResourceHandle RenderGraph::Write(RenderPassResource resource) {
			
	}

	RenderPassResourceHandle RenderGraph::Read(RenderPassResource resource) {
	
	}

	RenderPassResourceHandle RenderGraph::ReadWrite(RenderPassResource resource) {
	
	}

	RHI::Image* RenderGraph::GetImage(RenderPassResourceHandle handle) { 
		auto it = _resources[handle.id];
		if (it.handle.generation != handle.generation) {
			Logger::Error("RenderGraph", "Invalid resource access");
			return nullptr;
		}
		return it.attachment;
	}

	RHI::Image* RenderGraph::GetImage(const std::string& name) {
		if (!_resourceLookupTable.contains(name)) {
			Logger::Error("RenderGraph", "Invalid resource access, resource '{}' not registered", name);
			return nullptr;
		}

		auto it = _resourceLookupTable.find(name);
		
		return GetImage(_resources[it->second].handle);
	}


	void RenderGraph::Compile() {

	}

	void RenderGraph::Execute(Runtime::RHI::CommandBuffer * cmd) {

	}

	void RenderGraph::ClearPasses() {
		for (auto& stages : _passes) {
			for (auto& pass : stages) {
				Runtime::Mem::Allocator::Destroy(pass);
				pass = nullptr;
			}
		}
		_passes.clear();
		_resources.clear();
		_resourceLookupTable.clear();
	}
}