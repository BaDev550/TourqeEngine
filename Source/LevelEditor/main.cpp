#include <Engine/Core/Engine.h>

#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Window/WindowSubsystem.h>
#include <Engine/Widget/WidgetSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/World/WorldSubsystem.h>

#include <LevelEditor/LevelEditor.h>

int main() {
	{
		Tourqe::Engine::Engine engine{};
		engine.AddSubsystem<Tourqe::Engine::WindowSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::AssetSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::GraphicsSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::WorldSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::WidgetSubsystem>();
		engine.AddSubsystem<LevelEditor>();
		engine.Run();
	}
	return 0;
}