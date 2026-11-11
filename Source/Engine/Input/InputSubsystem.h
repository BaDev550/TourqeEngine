#pragma once

#include <Engine/Subsystem/Subsystem.h>
#include <Engine/Core/Core.h>
#include <bitset>

#include <Runtime/PAL/PlatformEvent.h>
#include <Runtime/PAL/KeyCodes.h>

namespace Tourqe::Engine {
	class ENGINE_API InputSubsystem final : public Subsystem {
	public:
		virtual Runtime::Result OnCreate(Engine* engine) final;
		virtual void OnDestroy() final;

		virtual void GetDependencyGraph(DependencyGraph& graph) final;

		bool IsKeyPressed(Runtime::KeyCode key) const;
		bool IsKeyReleased(Runtime::KeyCode key) const;
	private:
		void OnInputEventDetected(Runtime::PAL::PlatformEvent* event);

		std::bitset<256> _keys;
	};
}