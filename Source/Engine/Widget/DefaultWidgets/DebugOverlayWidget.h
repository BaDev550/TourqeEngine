#pragma once

#include <Engine/Widget/Widget.h>

namespace Tourqe::Engine {
	TCLASS();
	class DebugOverlayWidget final : public Widget {
		GENERATE_CLASS(DebugOverlayWidget);
	public:
		DebugOverlayWidget() = default;
		~DebugOverlayWidget() final = default;

		virtual void Draw() final;
	};
}