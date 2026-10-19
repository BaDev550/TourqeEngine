#pragma once

#include <Engine/Subsystem/Subsystem.h>
#include <Engine/Widget/Widget.h>
#include <unordered_map>

namespace Tourqe::Engine {
	class ENGINE_API WidgetSubsystem final : public Subsystem {
	public:
		virtual Runtime::Result OnCreate(Engine* engine) final;
		virtual void OnUpdate() final;
		virtual void OnDestroy() final;

		virtual void GetDependencyGraph(DependencyGraph& graph) final;
	private:
		std::unordered_map<std::string, i32> _widgetLookupTable;
		std::vector<Widget*> _widgets;
	};
}