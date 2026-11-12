#pragma once
#include <Widget/Widget.h>

TCLASS();
class EditorMenuBarWidget final : public Widget {
	GENERATE_CLASS(EditorMenuBarWidget);
public:
	EditorMenuBarWidget() = default;
	~EditorMenuBarWidget() final = default;

	virtual void Draw() final;
};