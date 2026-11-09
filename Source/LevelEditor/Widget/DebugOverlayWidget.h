#pragma once

#include "Widget.h"
#include <Engine/World/ECS/EntityDefinitions.h>

TCLASS();
class DebugOverlayWidget final : public Widget {
	GENERATE_CLASS(DebugOverlayWidget);
public:
	DebugOverlayWidget() = default;
	~DebugOverlayWidget() final = default;

	virtual void Draw() final;
private:
	Tourqe::Engine::EntityHandle _selectedEntity;
};