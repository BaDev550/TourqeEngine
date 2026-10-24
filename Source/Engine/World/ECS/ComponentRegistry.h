#pragma once

#include <Engine/World/ECS/ComponentStorage.h>
#include <Engine/World/ECS/EntityDefinitions.h>
#include <Runtime/Reflection/Type.h>

#include <unordered_map>

namespace Tourqe::Engine {
	// Forked: https://github.com/mtuncbilek95/Horizon/tree/pure-game-engine/Source/Engine/World/ECS
	class ENGINE_API ComponentRegistry final
	{
	public:
		ComponentRegistry() = default;
		~ComponentRegistry();
		ComponentRegistry(const ComponentRegistry&) = delete;
		ComponentRegistry& operator=(const ComponentRegistry&) = delete;

		ComponentStorage* GetOrCreateStorage(const Runtime::Reflection::Type* pType);
		ComponentStorage* FindStorage(std::type_index typeId) const;
		u32 FindSlot(std::type_index typeId) const;

		const std::vector<ComponentStorage*>& GetStorages() const { return _storages; }
		void Clear();
	private:
		std::unordered_map<std::type_index, u32> _slots;
		std::vector<ComponentStorage*> _storages;
	};
}