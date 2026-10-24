#include "WidgetSubsystem.h"

#include <Engine/Core/Engine.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Runtime/Memory/Memory.h>

#include <imgui.h>

namespace Tourqe::Engine {
    Runtime::Result WidgetSubsystem::OnCreate(Engine* engine) {
		Subsystem::OnCreate(engine);

        auto* reflectionSystem = _engine->GetReflectionSystem();

        reflectionSystem->ForEachWithBase<Widget>([this](const Runtime::Reflection::Type& type) {
            Widget* widget = static_cast<Widget*>(type.Create());
			if (!widget) {
				Logger::Info("WidgetSubsystem", "failed to create '{}' widget", widget->GetClassName().data());
				return;
			}

            _widgetLookupTable[widget->GetClassName().data()] = _widgets.size();
            _widgets.push_back(std::move(widget));
			Logger::Info("WidgetSubsystem", "'{}' widget added", widget->GetClassName().data());
            });

        return Runtime::Result();
    }

    void Tourqe::Engine::WidgetSubsystem::OnUpdate() {
        auto* renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();
    }

    void Tourqe::Engine::WidgetSubsystem::OnDestroy() {
		Subsystem::OnDestroy();
        for (auto& widget : _widgets) {
            Runtime::Mem::Allocator::Destroy(widget);
        }
    }

    void Tourqe::Engine::WidgetSubsystem::GetDependencyGraph(DependencyGraph& graph) {
        graph.Require<GraphicsSubsystem>(DependencyOrder::After);
		graph.Require<WorldSubsystem>(DependencyOrder::After);
    }
}