#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Subsystem/Subsystem.h>
#include <Engine/World/ECS/Scene.h>
#include <Engine/World/System.h>

namespace Tourqe::Engine { // All temp classes
	class ENGINE_API WorldSubsystem : public Subsystem {
	public:
		virtual Runtime::Result OnCreate(Engine* engine) override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
		
		virtual void GetDependencyGraph(DependencyGraph& graph) override;

		[[nodiscard]] Scene* GetActiveScene() { return _activeScene; }

		template<typename ST>
		ST* GetSystem() {
			for (auto* s : _systems) {
				if (s && typeid(*s) == typeid(ST)) 
					return static_cast<ST*>(s);
			}
			return nullptr;
		}
	private:
		std::vector<System*> _systems;
		Scene* _activeScene;
	}; 
}