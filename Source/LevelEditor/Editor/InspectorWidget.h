#pragma once
#include <Widget/Widget.h>

TCLASS();
class InspectorWidget final : public Widget {
	GENERATE_CLASS(InspectorWidget);
public:
	InspectorWidget() = default;
	~InspectorWidget() final = default;

	virtual void Draw() final;
};