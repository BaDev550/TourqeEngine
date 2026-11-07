#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Platform/PlatformSystem.h>
#include <Engine/Core/Engine.h>

namespace Tourqe::Engine {
	Runtime::Result WindowSubsystem::OnCreate(Engine* engine)
	{
		Subsystem::OnCreate(engine);
		auto platform = engine->GetPlatformSystem()->GetPlatform();

		Runtime::PAL::WindowDesc desc{};
		desc.width = 800;
		desc.height = 800;
		desc.title = "Engine Debug Window";
		_window = platform->CreateNewWindow(desc);
		if (!_window) return Runtime::Result("Failed to create window");

		return Runtime::Result();
	}

	void WindowSubsystem::OnUpdate() {
		_window->PollEvents();

		if (_window->ShouldClose()) {
			_engine->Shutdown("Window closed");
		}
	}

	void WindowSubsystem::OnDestroy() {
		Subsystem::OnDestroy();

		Runtime::Mem::Allocator::Destroy(_window);
	}
}