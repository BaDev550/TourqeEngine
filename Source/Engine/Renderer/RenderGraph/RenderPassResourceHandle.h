#pragma once

#include <Runtime/Definitions/Types.h>

namespace Tourqe::Engine {
	struct RenderPassResourceHandle {
		u32 id = u32_max;
		u32 generation = u32_max;

		[[nodiscard]] bool IsValid() const { return id != u32_max; }
	};

	enum class RenderPassResourceAccessFlag : u8 {
		Write,
		Read,
		ReadWrite
	};
}