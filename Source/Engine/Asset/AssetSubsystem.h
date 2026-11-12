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
			AssetT* Get(AssetHandle handle) {
			if (Asset* existing = TryAsset(handle)) {
				return static_cast<AssetT*>(existing);
			}
			if (AssetInCache(handle)) {
				return static_cast<AssetT*>(Load(handle));
			}
			return nullptr;
		}

		template<class AssetT> requires(std::is_base_of_v<Asset, AssetT>)
		AssetT* Get(const std::string& path) {
			std::string assetType = AssetT::GetStaticClassName().data();

			Runtime::Path inputPath(path);
			Runtime::Path sourcePath = inputPath.Relative("Assets");

			AssetHandle assetID;
			assetID.index = static_cast<u32>(std::hash<std::string>{}(sourcePath.GetPathStr()));

			if (Asset* existing = TryAsset(assetID)) { return static_cast<AssetT*>(existing); }

			if (AssetInCache(assetID)) {
				Logger::Info("AssetSubsystem", "Asset in registry loading...");
				return static_cast<AssetT*>(Load(assetID));
			}
			else {
				Runtime::Path targetPath = (COOKED_ASSET_DIR / Runtime::Path(path).ReplaceExtension(ASSET_EXTENSION));
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
					else {
						Logger::Error("AssetSubsystem", "Failed to cook asset: {}", result.GetMessage());
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

		AssetSource& GetSource(AssetHandle handle) { return _cachedAssetSources[handle]; }

		[[nodiscard]] const std::unordered_map<AssetHandle, Asset*>& GetAssets() const { return _assets; }
		[[nodiscard]] const std::unordered_map<AssetHandle, AssetSource>& GetAssetSources() const { return _cachedAssetSources; }
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