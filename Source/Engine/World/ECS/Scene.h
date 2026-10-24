#pragma once
#include <Engine/World/ECS/EntityFactory.h>
#include <Engine/World/ECS/ComponentRegistry.h>
#include <Engine/World/ECS/ComponentStorage.h>

#include <Runtime/Definitions/UUID.h>
#include <unordered_map>

namespace Tourqe::Engine {
	class WorldSubsystem;
	class ReflectionSystem;

	class ENGINE_API Scene final {
	public:
		Scene(ReflectionSystem* pReflection);
		~Scene();

		void Clear();

		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle handle);
		void DestroyEntity(usize index);
		b8 IsValid(EntityHandle handle);
		const ComponentRegistry& GetComponents() const { return _components; }

		BaseComponent* AddComponent(EntityHandle handl, std::type_index typeId);
		void RemoveComponent(EntityHandle handl, std::type_index typeId);
		BaseComponent* FindComponent(EntityHandle handl, std::type_index typeId) const;
		b8 HasComponent(EntityHandle handl, std::type_index typeId) const;

		template<typename T>
		T* AddComponent(EntityHandle handl, T value = T{}) {
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from BaseComponent");

			ComponentStorage* pStorage = ResolveStorage(typeid(T));
			if (!pStorage)
				return nullptr;

			return static_cast<T*>(AddComponentTo(pStorage, handl, &value));
		}

		template<typename T>
		void RemoveComponent(EntityHandle handl) {
			RemoveComponent(handl, typeid(T));
		}

		template<typename T>
		T* FindComponent(EntityHandle handl) const {
			return static_cast<T*>(FindComponent(handl, typeid(T)));
		}

		template<typename T>
		b8 HasComponent(EntityHandle handl) const {
			return HasComponent(handl, typeid(T));
		}

		template<typename... Ts, typename Fn>
		void ForEach(Fn&& func) {
			if constexpr (sizeof...(Ts) == 1) {
				ComponentStorage* pStorage = _components.FindStorage(typeid(Ts)...);

				if (!pStorage)
					return;

				for (usize i = 0; i < pStorage->GetCount(); i++)
					func(pStorage->GetEntityAt(i), *static_cast<Ts*>(pStorage->GetAt(i))...);
			}
			else {
				ComponentStorage* storages[] = { _components.FindStorage(typeid(Ts))... };

				for (ComponentStorage* pStorage : storages) {
					if (!pStorage)
						return;
				}

				ComponentStorage* pDriver = storages[0];

				for (ComponentStorage* pStorage : storages) {
					if (pStorage->GetCount() < pDriver->GetCount())
						pDriver = pStorage;
				}

				for (usize i = 0; i < pDriver->GetCount(); i++) {
					const EntityHandle entity = pDriver->GetEntityAt(i);

					[&](auto*... pComps)
						{
							if ((pComps && ...))
								func(entity, *pComps...);
						}(FindComponent<Ts>(entity)...);
				}
			}
		}
	private:
		ComponentStorage* ResolveStorage(std::type_index typeId);
		BaseComponent* AddComponentTo(ComponentStorage* pStorage, EntityHandle handl, void* pSource);

		ReflectionSystem* _reflection = nullptr;
		EntityFactory _entityFactory;
		ComponentRegistry _components;
	};
}