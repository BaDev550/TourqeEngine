#pragma once
#include <Engine/Core/Core.h>
#include <Engine/Subsystem/Subsystem.h>

#include <Engine/Asset/Asset.h>
#include <Engine/Asset/AssetStreamer.h>
#include <Engine/Asset/AssetSource.h>
#include <Runtime/IO/Path.h>
#include <unordered_set>

namespace Tourqe::Engine {
	class ENGINE_API AssetSubsystem final : public Subsystem {
	public:
		virtual Runtime::Result OnCreate(Engine* engine) override;
		virtual void OnDestroy() override;
		virtual void GetDependencyGraph(DependencyGraph& graph) override;

		template<class AssetT> requires(std::is_base_of_v<Asset, AssetT>)
		AssetT* Get(const std::string& path) {
			std::string assetType = AssetT::GetStaticClassName().data();

			Runtime::Path sourcePath = path;
			Runtime::Path targetPath = COOKED_ASSET_DIR / Runtime::Path(path).ReplaceExtension(ASSET_EXTENSION);
			AssetHandle assetID;
			assetID.index = _assets.size(); // HASH amk

			if (AssetInCache(assetID)) {
				Logger::Info("AssetSubsystem", "Asset in registry loading...");
				return static_cast<AssetT*>(Load(assetID));
			}
			else {
				AssetStreamer* streamer = GetAssetCookerOfType(assetType);

				if (streamer) {
					AssetSource source;
					source.handle = assetID;
					source.type = assetType;
					source.sourcePath = sourcePath.GetPathStr();
					source.cookedPath = targetPath.GetPathStr();

					Runtime::Result result = streamer->Cook(source);
					if (result) {
						_cachedAssetSources[source.handle] = source;

						AssetT* ast = static_cast<AssetT*>(Load(source.handle));

						SaveRegistry();
						return ast;
					}
					return nullptr;
				}
			}
			return nullptr;
		}

		Asset* TryAsset(AssetHandle handle) {
			if (!_assets.contains(handle)) return nullptr;
			return _assets[handle];
		}
	private:
		Asset* Load(AssetHandle handle);
		void Unload(AssetHandle handle);

		AssetStreamer* GetAssetCookerOfType(const std::string& type);
		b8 AssetInCache(AssetHandle handle);
		b8 AssetLoaded(AssetHandle handle);
		void SaveRegistry();
		void LoadRegistry();
		
		std::unordered_map<AssetHandle, AssetSource> _cachedAssetSources;

		std::vector<AssetStreamer*> _assetStreamers;
		std::unordered_map<std::string, u32> _assetStreamerLookupTable;

		std::unordered_map<AssetHandle, Asset*> _assets;
	};
}