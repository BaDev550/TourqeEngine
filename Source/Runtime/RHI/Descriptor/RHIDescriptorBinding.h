#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorType.h>
#include <Runtime/RHI/Shader/ShaderStage.h>

namespace Runtime::RHI {
	enum class DescriptorBindingFlags : u8 {
		None = 0,
		PartiallyBound = BIT(1),
		UpdateAfterBind = BIT(2)
	};
	CORE_DEFINE_ENUM_FLAG_OPERATORS(DescriptorBindingFlags);

	struct DescriptorBindingDesc {
		u32 count = 1;
		DescriptorType type = DescriptorType::SampledImage;
		ShaderStage stage = ShaderStage::All;
		DescriptorBindingFlags flags = DescriptorBindingFlags::None;
	private:
		u32 binding = 0;

		friend class DescriptorHeap;
		friend class DescriptorSetLayout;
	};
}