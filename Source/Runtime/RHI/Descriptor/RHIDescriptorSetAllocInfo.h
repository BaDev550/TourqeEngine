#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorType.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorBinding.h>
#include <vector>

namespace Runtime::RHI {
	struct DescriptorSetAllocInfo {
		std::vector<DescriptorBindingDesc> bindings;
	};
}