#pragma once

#include <Runtime/Definitions/Types.h>

namespace Runtime::Reflection {
	enum class PropertyFlags : u32 {
		None = 0,
		Edit = BIT(0),
		ReadOnly = BIT(1),
		Serialize = BIT(2),
		Transient = BIT(3),
		Color = BIT(4)
	};
	CORE_DEFINE_ENUM_FLAG_OPERATORS(PropertyFlags)
}