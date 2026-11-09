#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Runtime/Definitions/Types.h>
#include <Runtime/Containers/UUID.h>

namespace Tourqe::Engine {
	TCLASS();
	class IdenityComponent final : public BaseComponent {
		GENERATE_CLASS(IdenityComponent)
	public:
		TPROP();
		std::string name;

		TPROP();
		Runtime::UUID uuid;
	};
}