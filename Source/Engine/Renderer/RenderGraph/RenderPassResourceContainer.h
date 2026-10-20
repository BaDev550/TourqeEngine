#pragma once

#include <Runtime/Definitions/Types.h>

#include <Engine/Renderer/RenderGraph/RenderPassResourceHandle.h>
#include <Runtime/RHI/Image/RHIImageDesc.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <string>
#include <vector>

namespace Tourqe::Engine {
	struct RenderPassResource {
		std::string name;
		RenderPassResourceAccessFlag access;
		Runtime::RHI::Image* attachment = nullptr;
		
		RenderPassResource() = default;
		RenderPassResource(const std::string& name, Runtime::RHI::Image* image) : name(name), attachment(image) {}
	private:
		b8 ready = false;

		friend class RenderGraph;
	};

	class RenderPassResourceContainer final {
	public:
		RenderPassResourceContainer() = default;

		RenderPassResourceHandle Write(RenderPassResource resource);
		RenderPassResourceHandle Read(RenderPassResource resource);
		RenderPassResourceHandle ReadWrite(RenderPassResource resource);

		[[nodiscard]] RenderPassResource& GetResource(usize index) { return _resources[index]; }
		[[nodiscard]] const std::vector<RenderPassResource>& GetResources() const { return _resources; }
	private:
		std::vector<RenderPassResource> _resources;
	};
}