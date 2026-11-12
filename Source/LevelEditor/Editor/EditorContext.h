#pragma once
#include <Engine/World/ECS/EntityDefinitions.h>
#include <Engine/Asset/Asset.h>

namespace Tourqe::LevelEditor {
	struct EditorContext {
		static EditorContext& Get() {
			static EditorContext s_instance;
			return s_instance;
		}

		Engine::EntityHandle selectedEntity = { 0xFFFFFFFF, 0u };

		bool showHierarchy = true;
		bool showInspector = true;
		bool showAssetBrowser = true;
		bool showProfiler = true;
	};
}