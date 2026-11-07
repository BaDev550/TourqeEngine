#pragma once
#include <Engine/Asset/AssetLoadStat.h>

#include <Runtime/Reflection/ReflectObject.h>
#include <Runtime/Definitions/UUID.h>
#include <Runtime/Definitions/Handle.h>

namespace Tourqe::Engine {
	class AssetStreamer;
	class Asset;
	using AssetHandle = Runtime::Handle<Asset>;
	
	struct AssetHeader {
		u32 magic = ASSET_MAGIC;
		AssetHandle handle;
		c8 typeName[256] = "\0";
		u64 metaSize = 0;
		u64 dataOffset = 0;
		u64 dataSize = 0;
	};

	TCLASS();
	class Asset : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(Asset);
	public:
		Asset() = default;
		virtual ~Asset() = default;

		[[nodiscard]] const AssetHandle GetAssetHandle() const { return _assetHandle; }
		void SetAssetHandle(AssetHandle handle) { _assetHandle = handle; }

		[[nodiscard]] AssetState GetAssetState() const { return _assetState; }
		void SetAssetState(AssetState state) { _assetState = state; }
	private:
		AssetHandle _assetHandle;
		AssetState _assetState = AssetState::InDisk;
	};
}