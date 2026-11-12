#pragma once
#include <Widget/Widget.h>

TCLASS();
class ConsolleWidget final : public Widget {
	GENERATE_CLASS(ConsolleWidget);
public:
	ConsolleWidget() = default;
	~ConsolleWidget() final = default;

	virtual void Draw() final;
};