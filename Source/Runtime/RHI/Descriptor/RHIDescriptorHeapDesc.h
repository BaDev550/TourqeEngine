#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorBinding.h>
#include <vector>

namespace Runtime::RHI {
	enum class DescriptorHeapCreateFlags : u8 {
		None = 0,
		UpdateAfterBind = BIT(1),
		FreeDescriptorSet = BIT(2)
	};
	CORE_DEFINE_ENUM_FLAG_OPERATORS(DescriptorHeapCreateFlags);

	struct DescriptorHeapSize {
		DescriptorType type = DescriptorType::SampledImage;
		u32 count = 0;
	};

	struct DescriptorHeapDesc {
		u32 maxDescriptorSetAllocation = 1000;
		DescriptorHeapCreateFlags flags = DescriptorHeapCreateFlags::None;

		std::vector<DescriptorHeapSize> sizes;
	};
}