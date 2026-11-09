#pragma once
#include <Engine/Core/Core.h>

namespace Runtime::RHI {
	enum class ImageLayout : u8 {
		Undefined = 0,
		UnorderedAccess,
		PresentSrc,
		TransferDst,
		TransferSrc,
		ColorAttachment,
		DepthAttachment,
		ShaderReadOnly
	};
}