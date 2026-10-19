#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorType.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorSetAllocInfo.h>

#include <Runtime/RHI/Object/RHIObject.h>

namespace Runtime::RHI {
	class Image;
	class Buffer;

	class DescriptorSet : public Object {
	public:
		using Object::Object;
		virtual ~DescriptorSet() = default;
		
		virtual void WriteImage(u32 binding, u32 arrayIndex, DescriptorType type, Image* image) = 0;
		virtual void WriteBuffer(u32 binding, u32 arrayIndex, DescriptorType type, Buffer* buffer) = 0;
	};
}