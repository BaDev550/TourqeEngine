#pragma once

#include <Runtime/Definitions/Types.h>

namespace Runtime::RHI {
	enum class PipelineBindPoint : u8 {
		Graphics = 0,
		Compute
	};
}