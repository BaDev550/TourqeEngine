#pragma once

#include <Runtime/RHI/Pipeline/RHIPipelineDesc.h>
#include <Runtime/RHI/Object/RHIObject.h>

namespace Runtime::RHI {
	class CommandBuffer;

	class Pipeline : public Object {
	public:
		using Object::Object;
		virtual ~Pipeline() = default;

		[[nodiscard]] virtual const PipelineDesc& GetDesc() const = 0;
	};
}