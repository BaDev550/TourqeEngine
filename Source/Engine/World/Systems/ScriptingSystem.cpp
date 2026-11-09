#include "ScriptingSystem.h"
#include <Engine/Reflection/ReflectionSystem.h>

namespace Tourqe::Engine {
	Runtime::Result ScriptingSystem::OnCreate() {
		return Runtime::Result();
	}

	void ScriptingSystem::OnUpdate(Scene& scene) {
		for (auto* gameObject : _gameObjects) {
			gameObject->OnUpdate();
		}
	}

	void ScriptingSystem::OnDestroy() {
		for (auto* gameObject : _gameObjects) {
			gameObject->OnDestroy();
			Runtime::Mem::Allocator::Destroy(gameObject);
		}
	}

	void ScriptingSystem::RegisterGameObjects(Scene& scene)
	{
		auto* reflectionSystem = Engine::Get()->GetReflectionSystem();

		reflectionSystem->ForEachWithBase<GameObject>([this, &scene](const Runtime::Reflection::Type& typeInfo) {
			GameObject* gameObject = static_cast<GameObject*>(typeInfo.Create());
			gameObject->OnInitialized(&scene);
			gameObject->OnCreate();
			_gameObjects.push_back(gameObject);
			});
	}
}