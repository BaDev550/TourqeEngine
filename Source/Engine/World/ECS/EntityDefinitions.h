#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Containers/UUID.h>
#include <bitset>

namespace Tourqe::Engine {
	static constexpr u32 EstimatedEntities = 2048;
	static constexpr u32 MaxComponents = 256;
	
	struct EntityHandle {
		u32 index = 0;
		u32 generation = 0;

		b8 IsValid() const { return index != 0xFFFFFFFF; }

		bool operator==(const EntityHandle&) const noexcept = default;
		bool operator!=(const EntityHandle&) const noexcept = default;
	};

	using Signature = std::bitset<MaxComponents>;
}

namespace std {
	template<>
	struct hash<Tourqe::Engine::EntityHandle> {
		size_t operator()(const Tourqe::Engine::EntityHandle& handle) const noexcept {
			const u64 packed = (static_cast<u64>(handle.generation) << 32) | static_cast<u64>(handle.index);
			return hash<u64>()(packed);
		}
	};
}