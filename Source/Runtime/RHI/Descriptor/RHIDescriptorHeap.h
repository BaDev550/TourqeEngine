#pragma once

#include <Runtime/RHI/Descriptor/RHIDescriptorHeapDesc.h>
#include <Runtime/RHI/Descriptor/RHIDescriptorSet.h>
#include <Runtime/RHI/Object/RHIObject.h>
#include <list>

namespace Runtime::RHI {
	class DescriptorHeap : public Object {
	public:
		using Object::Object;
		virtual ~DescriptorHeap() = default;

		virtual DescriptorSet* AllocDescriptorSet(const DescriptorSetAllocInfo& allocInfo) = 0;
		
		[[nodiscard]] virtual std::vector<DescriptorSet*>& GetDescriptorSets() noexcept = 0;
		[[nodiscard]] virtual const DescriptorHeapDesc& GetDesc() const noexcept = 0;
	};
}