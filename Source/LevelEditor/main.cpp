#include <Engine/Core/Engine.h>

#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/World/WorldSubsystem.h>

#include <LevelEditor/LevelEditor.h>
#include <LevelEditor/Widget/WidgetSubsystem.h>

#include <reflectManifest.h>

int main() {
	{
		Tourqe::Engine::Engine engine{};
		engine.AddSubsystem<Tourqe::Engine::WindowSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::AssetSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::GraphicsSubsystem>();
		engine.AddSubsystem<Tourqe::Engine::WorldSubsystem>();
		engine.AddSubsystem<LevelEditor>();
		engine.AddSubsystem<WidgetSubsystem>();
		engine.Run();
	}
	return 0;
}