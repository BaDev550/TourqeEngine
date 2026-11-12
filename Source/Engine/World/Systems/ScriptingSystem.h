#pragma once

#include <Runtime/Definitions/Types.h>
#include <Engine/World/System.h>
#include <Engine/Scripting/GameObject.h>

namespace Tourqe::Engine {
	class ENGINE_API ScriptingSystem final : public System {
	public:
		virtual Runtime::Result OnCreate() override;
		virtual void OnUpdate(Scene& scene) override;
		virtual void OnDestroy() override;

		void RegisterGameObjects(Scene& scene);

		GameObject* GetGameObject(EntityHandle handle) const;

		[[nodiscard]] b8 IsScriptable(EntityHandle handle) const;
	private:
		std::vector<GameObject*> _gameObjects;
		std::unordered_map<EntityHandle, i32> _gameObjectLookupTable;
	};
}