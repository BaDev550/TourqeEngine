#include "ComponentRegistry.h"

#include <Runtime/Definitions/Logger.h>
#include <Runtime/Memory/Memory.h> // memory ismini sikicem

namespace Tourqe::Engine {
	ComponentRegistry::~ComponentRegistry() {
		Clear();
	}

	ComponentStorage* ComponentRegistry::GetOrCreateStorage(const Runtime::Reflection::Type* pType) {
		if (!pType) {
			Logger::Error("ComponentRegistry", "Cannot create a storage for a null type");
			return nullptr;
		}

		auto it = _slots.find(pType->GetTypeIndex());
		if (it != _slots.end())
			return _storages[it->second];

		if (_storages.size() >= MaxComponents) {
			Logger::Error("ComponentRegistry", "Component budget of {} is exhausted", MaxComponents);
			Logger::Warn("Developer Info", "How the fuck did you need more than 256 component slots?? Re-assess your design god damn it."); // facks
			return nullptr;
		}

		auto* pStorage = Runtime::Mem::Allocator::Construct<ComponentStorage>(pType);
		if (!pStorage) {
			Logger::Error("ComponentRegistry", "Storage for '{}' could not be allocated", pType->GetName());
			return nullptr;
		}

		const u32 slot = (u32)_storages.size();
		pStorage->SetSlot(slot);
		_slots[pType->GetTypeIndex()] = slot;
		_storages.push_back(pStorage);

		return pStorage;
	}

	ComponentStorage* ComponentRegistry::FindStorage(std::type_index typeId) const
	{
		auto it = _slots.find(typeId);
		if (it == _slots.end())
			return nullptr;

		return _storages[it->second];
	}

	u32 ComponentRegistry::FindSlot(std::type_index typeId) const
	{
		auto it = _slots.find(typeId);
		if (it == _slots.end())
			return u32_max;

		return it->second;
	}

	void ComponentRegistry::Clear()
	{
		for (ComponentStorage* pStorage : _storages)
			Runtime::Mem::Allocator::Destroy(pStorage);

		_storages.clear();
		_slots.clear();
	}
}