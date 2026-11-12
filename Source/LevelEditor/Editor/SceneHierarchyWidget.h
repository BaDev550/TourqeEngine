#pragma once
#include <Widget/Widget.h>

TCLASS();
class SceneHierarchyWidget final : public Widget {
	GENERATE_CLASS(SceneHierarchyWidget);
public:
	SceneHierarchyWidget() = default;
	~SceneHierarchyWidget() final = default;

	virtual void Draw() final;

private:
	char _searchFilter[128] = {};
};