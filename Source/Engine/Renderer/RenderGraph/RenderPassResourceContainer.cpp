#include "RenderPassResourceContainer.h"

namespace Tourqe::Engine {
	void RenderPassResourceContainer::Write(RenderPassResource resource) {
		resource.access = RenderPassResourceAccessFlag::Write;
		_resources.push_back(std::move(resource));
	}

	void RenderPassResourceContainer::Read(RenderPassResource resource) {
		resource.access = RenderPassResourceAccessFlag::Read;
		_resources.push_back(std::move(resource));
	}

	void RenderPassResourceContainer::ReadWrite(RenderPassResource resource) {
		resource.access = RenderPassResourceAccessFlag::ReadWrite;
		_resources.push_back(std::move(resource));
	}
}