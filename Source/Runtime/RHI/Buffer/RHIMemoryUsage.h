#pragma once
#include <Engine/Core/Core.h>

namespace Runtime::RHI {
	enum class MemoryUsage : u8 {
		Auto,
		GPU,
		CPU,
		CPUToGPU
	};

	enum class AllocationFlagBits : u8 {
		None = 0,
		HostAccessSequentialWrite = BIT(0),
		HostAccessRandom = BIT(1),
		CreateMapped = BIT(2)
	};
	CORE_DEFINE_ENUM_FLAG_OPERATORS(AllocationFlagBits);
}