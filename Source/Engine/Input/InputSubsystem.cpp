#include "InputSubsystem.h"
#include <Engine/Core/Engine.h>
#include <Engine/Platform/Window/WindowSubsystem.h>

#include <Runtime/PAL/PlatformEvent.h>

namespace Tourqe::Engine {
	Runtime::Result InputSubsystem::OnCreate(Engine* engine) {
		Subsystem::OnCreate(engine);

		auto* windowSubsystem = engine->GetSubsystem<WindowSubsystem>();
		if (!windowSubsystem) return Runtime::Result("Cant access window subsystem!");

		auto* window = windowSubsystem->GetWindow();
		window->GetDesc().eventCallback = [&](Runtime::PAL::PlatformEvent* event) -> void { OnInputEventDetected(event); };

		return Runtime::Result();
	}

	void InputSubsystem::OnDestroy() {
		auto* windowSubsystem = _engine->GetSubsystem<WindowSubsystem>();
		if (!windowSubsystem) return;

		auto* window = windowSubsystem->GetWindow();
		window->GetDesc().eventCallback = nullptr;
	}

	void InputSubsystem::GetDependencyGraph(DependencyGraph& graph) {
		graph.Require<WindowSubsystem>(DependencyOrder::After);
	}

	bool InputSubsystem::IsKeyPressed(Runtime::KeyCode key) const {
		u16 keyindex = static_cast<u16>(key);
		return _keys[keyindex];
	}

	bool InputSubsystem::IsKeyReleased(Runtime::KeyCode key) const {
		u16 keyindex = static_cast<u16>(key);
		return !_keys[keyindex];
	}

	void InputSubsystem::OnInputEventDetected(Runtime::PAL::PlatformEvent* event) {
		switch (event->GetType())
		{
		case Runtime::PAL::PlatformEventType::KeyPressed: {
			Runtime::PAL::PlatformKeyPreseedEvent* keyevent = static_cast<Runtime::PAL::PlatformKeyPreseedEvent*>(event);
			u16 keyindex = static_cast<u16>(keyevent->GetKeyCode());

			if (keyevent) {
				_keys[keyindex] = true;
			}
			break;
		}
		case Runtime::PAL::PlatformEventType::KeyReleased: {
			Runtime::PAL::PlatformKeyReleasedEvent* keyevent = static_cast<Runtime::PAL::PlatformKeyReleasedEvent*>(event);
			u16 keyindex = static_cast<u16>(keyevent->GetKeyCode());

			if (keyevent && _keys[keyindex]) {
				_keys[keyindex] = false;
			}
			break;
		}
		default:
			break;
		}
	}
}