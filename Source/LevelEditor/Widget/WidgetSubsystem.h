#pragma once

#include <Engine/Subsystem/Subsystem.h>
#include <Widget/Widget.h>
#include <unordered_map>

class WidgetSubsystem final : public Tourqe::Engine::Subsystem {
public:
	virtual Runtime::Result OnCreate(Tourqe::Engine::Engine* engine) final;
	virtual void OnUpdate() final;
	virtual void OnDestroy() final;

	virtual void GetDependencyGraph(Tourqe::Engine::DependencyGraph& graph) final;
private:
	std::unordered_map<std::string, i32> _widgetLookupTable;
	std::vector<Widget*> _widgets;
};