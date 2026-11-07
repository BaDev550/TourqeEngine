#pragma once

#include <Runtime/Definitions/Types.h>
#include <Engine/Subsystem/Subsystem.h>
#include <Runtime/PAL/PlatformContext.h>

namespace Tourqe::Engine {
	class ENGINE_API PlatformSystem final {
	public:
		PlatformSystem();
		~PlatformSystem();

		[[nodiscard]] Runtime::PAL::PlatformContext* GetPlatform() const { return _platformContext; }
	private:
		Runtime::PAL::PlatformContext* _platformContext = nullptr;
	};
}