#pragma once
#include <Widget/Widget.h>

TCLASS();
class AssetBrowserWidget final : public Widget {
	GENERATE_CLASS(AssetBrowserWidget);
public:
	AssetBrowserWidget() = default;
	~AssetBrowserWidget() final = default;

	virtual void Draw() final;

private:
	char _searchFilter[128] = {};
	bool _onlyShowLoaded = false;
};