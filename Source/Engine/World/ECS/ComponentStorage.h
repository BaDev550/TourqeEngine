#pragma once

#include <Engine/Core/Core.h>
#include <Engine/World/Components/BaseComponent.h>
#include <Engine/World/ECS/EntityDefinitions.h>
#include <Runtime/Reflection/Type.h>

namespace Tourqe::Engine {
	// Forked: https://github.com/mtuncbilek95/Horizon/tree/pure-game-engine/Source/Engine/World/ECS
	class ENGINE_API ComponentStorage final {
	public:
		ComponentStorage(const Runtime::Reflection::Type* type);
		~ComponentStorage();

		BaseComponent* Insert(EntityHandle handle, void* source);
		BaseComponent* InsertBlind(EntityHandle handle);
		BaseComponent* Find(EntityHandle handle) const;
		void Remove(EntityHandle handle);
		b8 Contains(EntityHandle handle) const;

		BaseComponent* GetAt(usize index) const;
		EntityHandle GetEntityAt(usize index) const { return _entity[index]; }
		usize GetCount() const { return _count; }

		const Runtime::Reflection::Type* GetType() const { return _type; }
		std::type_index GetComponentTypeId() const { return _type->GetTypeIndex(); }

		u32 GetSlot() const { return m_slot; }
		void SetSlot(u32 slot) { m_slot = slot; }
	private:
		void* SlotAt(usize denseIndex) const;
		void* AcquireSlot(EntityHandle handl);
		void EnsureCapacity(usize required);
		void EnsureSparse(u32 index);
	private:
		const Runtime::Reflection::Type* _type = nullptr;
		u8* _data = nullptr;
		usize _count = 0;
		usize _capacity = 0;

		std::vector<EntityHandle> _entity;
		std::vector<u32> _sparse;

		u32 m_slot = u32_max;
	};
}