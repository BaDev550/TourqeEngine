#include "ComponentStorage.h"

#include <Runtime/Memory/Memory.h>
#include <Runtime/Definitions/Logger.h>

namespace Tourqe::Engine {
	namespace {
		static constexpr usize sInitialCapacity = 16;
	}

	ComponentStorage::ComponentStorage(const Runtime::Reflection::Type* type) : _type(type) {
		_sparse.resize(EstimatedEntities);

		for (usize i = 0; i < _sparse.size(); i++) _sparse[i] = u32_max;
	}

	ComponentStorage::~ComponentStorage() {
		for (usize i = 0; i < _count; i++)
			_type->DestructAt(SlotAt(i));

		if (_data)
			Runtime::Mem::Allocator::Free(_data);
	}

	BaseComponent* ComponentStorage::Insert(EntityHandle handl, void* pSource) {
		if (!_type->CanMove()) {
			Logger::Error("ComponentStorage", "'{}' is not move constructible", _type->GetName());
			return nullptr;
		}

		void* pSlot = AcquireSlot(handl);
		if (!pSlot)
			return Find(handl);

		_type->MoveAt(pSlot, pSource);
		return static_cast<BaseComponent*>(pSlot);
	}

	BaseComponent* ComponentStorage::InsertBlind(EntityHandle handl) {
		if (!_type->CanConstruct()) {
			Logger::Error("ComponentStorage", "'{}' is not default constructible", _type->GetName());
			return nullptr;
		}

		void* pSlot = AcquireSlot(handl);
		if (!pSlot)
			return Find(handl);

		_type->ConstructAt(pSlot);
		return static_cast<BaseComponent*>(pSlot);
	}

	BaseComponent* ComponentStorage::Find(EntityHandle handl) const {
		const u32 index = (u32)handl.index;
		if (index >= _sparse.size())
			return nullptr;

		const u32 denseIndex = _sparse[index];
		if (denseIndex == u32_max)
			return nullptr;

		return GetAt(denseIndex);
	}

	void ComponentStorage::Remove(EntityHandle handl) {
		const u32 index = (u32)handl.index;

		if (index >= _sparse.size() || _sparse[index] == u32_max)
			return;

		const u32 removeAt = _sparse[index];
		const u32 lastAt = (u32)_count - 1;

		void* pRemove = SlotAt(removeAt);
		_type->DestructAt(pRemove);

		if (removeAt != lastAt) {
			void* pLast = SlotAt(lastAt);
			_type->MoveAt(pRemove, pLast);
			_type->DestructAt(pLast);

			_entity[removeAt] = _entity[lastAt];
			_sparse[(u32)_entity[removeAt].index] = removeAt;
		}

		_entity.pop_back();
		_sparse[index] = u32_max;
		_count--;
	}

	b8 ComponentStorage::Contains(EntityHandle handl) const {
		const u32 index = (u32)handl.index;

		if (index >= _sparse.size())
			return false;

		return _sparse[index] != u32_max;
	}

	BaseComponent* ComponentStorage::GetAt(usize denseIndex) const {
		return static_cast<BaseComponent*>(SlotAt(denseIndex));
	}

	void* ComponentStorage::SlotAt(usize denseIndex) const {
		return _data + denseIndex * _type->GetSize();
	}

	void* ComponentStorage::AcquireSlot(EntityHandle handl) {
		const u32 index = (u32)handl.index;
		EnsureSparse(index);

		if (_sparse[index] != u32_max) {
			Logger::Warn("ComponentStorage", "Entity {} already owns '{}'", index, _type->GetName());
			return nullptr;
		}

		EnsureCapacity(_count + 1);

		_sparse[index] = (u32)_count;
		_entity.push_back(handl);

		void* pSlot = SlotAt(_count);
		_count++;

		return pSlot;
	}

	void ComponentStorage::EnsureCapacity(usize required)
	{
		if (required <= _capacity)
			return;

		usize newCapacity = _capacity == 0 ? sInitialCapacity : _capacity;

		while (newCapacity < required)
			newCapacity *= 2;

		const usize stride = _type->GetSize();
		u8* pNewData = (u8*)Runtime::Mem::Allocator::Allocate(newCapacity * stride, _type->GetAlignment());

		for (usize i = 0; i < _count; i++) {
			void* pOld = SlotAt(i);
			void* pNew = pNewData + i * stride;

			_type->MoveAt(pNew, pOld);
			_type->DestructAt(pOld);
		}

		if (_data)
			Runtime::Mem::Allocator::Free(_data);

		_data = pNewData;
		_capacity = newCapacity;
	}

	void ComponentStorage::EnsureSparse(u32 index) {
		const usize oldCount = _sparse.size();

		if (index < oldCount)
			return;

		usize newCount = oldCount == 0 ? EstimatedEntities : oldCount;

		while (newCount <= index)
			newCount *= 2;

		_sparse.resize(newCount);

		for (usize i = oldCount; i < newCount; i++)
			_sparse[i] = u32_max;
	}
}