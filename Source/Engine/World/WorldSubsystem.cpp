#include "WorldSubsystem.h"

#include <Engine/World/Systems/SceneRenderer2D.h>
#include <Engine/World/Systems/ScriptingSystem.h>

#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/MT/JobSystem.h>

namespace Tourqe::Engine {
	Runtime::Result WorldSubsystem::OnCreate(Engine* engine) {
		Subsystem::OnCreate(engine);

		auto* reflectionSystem = Engine::Get()->GetReflectionSystem();

		_activeScene = Runtime::Mem::Allocator::Construct<Scene>(reflectionSystem);

		SceneRenderer2DSystem* renderSystem = Runtime::Mem::Allocator::Construct<SceneRenderer2DSystem>();
		renderSystem->SetEngine(engine);
		renderSystem->OnCreate();

		ScriptingSystem* scriptingSystem = Runtime::Mem::Allocator::Construct<ScriptingSystem>();
		scriptingSystem->SetEngine(engine);
		scriptingSystem->OnCreate();
		scriptingSystem->RegisterGameObjects(*_activeScene);

		_systems.push_back(std::move(renderSystem));
		_systems.push_back(std::move(scriptingSystem));

		return Runtime::Result();
	}

	void WorldSubsystem::OnUpdate() {
		auto* jobSystem = Engine::Get()->GetJobSystem();

		for (auto& system : _systems) {
			system->OnUpdate(*_activeScene);
		}
	}

	void WorldSubsystem::OnDestroy() {
		Subsystem::OnDestroy();

		for (auto& system : _systems) {
			system->OnDestroy();
			Runtime::Mem::Allocator::Destroy(system);
		}
		Runtime::Mem::Allocator::Destroy(_activeScene);
	}

	void WorldSubsystem::GetDependencyGraph(DependencyGraph& graph) {
		graph.Require<AssetSubsystem>(DependencyOrder::After);
		graph.Require<GraphicsSubsystem>(DependencyOrder::After);
	}
}