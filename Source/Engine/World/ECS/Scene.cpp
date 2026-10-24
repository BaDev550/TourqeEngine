#include "Scene.h"

#include <Engine/Core/Engine.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Runtime/Reflection/Type.h>

namespace Tourqe::Engine {
	EntityHandle Scene::CreateEntity() {
		return _entityFactory.Create();
	}

	void Scene::DestroyEntity(EntityHandle handle) {
		if (!_entityFactory.IsValid(handle))
			return;

		const Signature* pSignature = _entityFactory.GetSignatureOf(handle);
		for (ComponentStorage* pStorage : _components.GetStorages()) {
			if (pSignature->test(pStorage->GetSlot()))
				pStorage->Remove(handle);
		}

		_entityFactory.Destroy(handle);
	}

	void Scene::DestroyEntity(usize index) {
		EntityHandle handle = _entityFactory.GetHandleAt(index);
		DestroyEntity(handle);
	}

	b8 Scene::IsValid(EntityHandle handle) { return _entityFactory.IsValid(handle); }

	Scene::Scene(ReflectionSystem* pReflection) : _reflection(pReflection) {}
	Scene::~Scene() {}

	void Scene::Clear() {
		for (u32 index = 0; index < _entityFactory.GetLatestIndex(); index++) {
			const EntityHandle handl = _entityFactory.GetHandleAt(index);

			if (!_entityFactory.IsValid(handl))
				continue;

			_entityFactory.Destroy(handl);
		}

		_components.Clear();
	}

	BaseComponent* Scene::AddComponent(EntityHandle handl, std::type_index typeId)
	{
		ComponentStorage* pStorage = ResolveStorage(typeId);
		if (!pStorage)
			return nullptr;

		return AddComponentTo(pStorage, handl, nullptr);
	}

	void Scene::RemoveComponent(EntityHandle handl, std::type_index typeId) {
		if (!_entityFactory.IsValid(handl))
			return;

		ComponentStorage* pStorage = _components.FindStorage(typeId);
		if (!pStorage)
			return;

		pStorage->Remove(handl);

		if (Signature* pSignature = _entityFactory.GetSignatureOf(handl))
			pSignature->reset(pStorage->GetSlot());
	}

	BaseComponent* Scene::FindComponent(EntityHandle handl, std::type_index typeId) const {
		if (!_entityFactory.IsValid(handl))
			return nullptr;

		ComponentStorage* pStorage = _components.FindStorage(typeId);
		if (!pStorage)
			return nullptr;

		return pStorage->Find(handl);
	}

	b8 Scene::HasComponent(EntityHandle handl, std::type_index typeId) const {
		if (!_entityFactory.IsValid(handl))
			return false;

		const u32 slot = _components.FindSlot(typeId);
		if (slot == u32_max)
			return false;

		return _entityFactory.GetSignatureOf(handl)->test(slot);
	}

	ComponentStorage* Scene::ResolveStorage(std::type_index typeId) {
		if (ComponentStorage* pStorage = _components.FindStorage(typeId))
			return pStorage;

		const Runtime::Reflection::Type* pType = _reflection->GetType(typeId);
		if (!pType)
		{
			Logger::Error("Scene", "Component type is not reflected. Did you forget TCLASS or to regenerate?");
			return nullptr;
		}

		return _components.GetOrCreateStorage(pType);
	}

	BaseComponent* Scene::AddComponentTo(ComponentStorage* pStorage, EntityHandle handl, void* pSource) {
		if (!_entityFactory.IsValid(handl)) {
			Logger::Warn("Scene", "AddComponent on a dead or stale entity");
			return nullptr;
		}

		BaseComponent* pComponent = pSource ? pStorage->Insert(handl, pSource) : pStorage->InsertBlind(handl);
		if (!pComponent)
			return nullptr;

		_entityFactory.GetSignatureOf(handl)->set(pStorage->GetSlot());

		return pComponent;
	}
}