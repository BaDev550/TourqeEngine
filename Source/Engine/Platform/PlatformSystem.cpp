#include "PlatformSystem.h"
#include <Runtime/Memory/Allocator.h>

namespace Tourqe::Engine {
	PlatformSystem::PlatformSystem() {
		_platformContext = Runtime::PAL::CreatePlatformContext();

		if (!_platformContext) {
			Logger::Error("PlatformSubsystem", "Failed to create platform context");
			return;
		}
	}

	PlatformSystem::~PlatformSystem() {
		if (_platformContext) {
			Runtime::Mem::Allocator::Destroy(_platformContext);
			_platformContext = nullptr;
		}
	}
}