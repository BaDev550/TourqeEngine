#include "AssetBrowserWidget.h"
#include "EditorContext.h"
#include <Engine/Core/Engine.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Platform/PlatformSystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/Asset/Shader/Shader.h>
#include <imgui.h>
#include <string_view>

using namespace Tourqe::Engine;
using namespace Tourqe::LevelEditor;

void AssetBrowserWidget::Draw() {
	auto& ctx = EditorContext::Get();
	if (!ctx.showAssetBrowser) return;

	auto* window = Engine::Get()->GetSubsystem<WindowSubsystem>()->GetWindow();
	float winHeight = static_cast<float>(window->GetHeight()) * 0.35f;
	float winWidth = static_cast<float>(window->GetWidth()) - 340.0f;

	ImGui::SetNextWindowPos(ImVec2(0.0f, static_cast<float>(window->GetHeight()) - winHeight), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Asset Browser", &ctx.showAssetBrowser)) {
		auto* assetSystem = Engine::Get()->GetSubsystem<AssetSubsystem>();
		if (!assetSystem) {
			ImGui::TextDisabled("AssetSubsystem unavailable");
			ImGui::End();
			return;
		}

		if (ImGui::Button("+ Import Asset...")) {
			auto* platform = Engine::Get()->GetPlatformSystem()->GetPlatform();
			auto* window = Engine::Get()->GetSubsystem<WindowSubsystem>()->GetWindow();

			const char* filter =
				"Supported Assets (*.png;*.jpg;*.hlsl)\0*.png;*.jpg;*.jpeg;*.bmp;*.glsl\0"
				"Textures (*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.jpeg;*.bmp\0"
				"Shaders (*.glsl)\0*.glsl\0"
				"All Files (*.*)\0*.*\0";

			Runtime::Path selectedPath = platform->OpenFileDialog(filter, reinterpret_cast<void*>(window->GetHandle()));
			std::string pathStr = selectedPath.GetPathStr();

			if (!pathStr.empty()) {
				std::string ext = selectedPath.GetExtension();
				if (ext == ".png" || ext == "png" || ext == ".jpg" || ext == "jpg" || ext == ".bmp" || ext == "bmp") {
					assetSystem->Get<Texture2D>(pathStr);
				}
				else if (ext == ".glsl" || ext == "glsl") {
					assetSystem->Get<Shader>(pathStr);
				}
			}
		}
		ImGui::SameLine();

		const auto& assets = assetSystem->GetAssets();
		const auto& sources = assetSystem->GetAssetSources();

		ImGui::Text("Registered: %zu | Resident in RAM: %zu", sources.size(), assets.size());
		ImGui::SameLine();
		ImGui::Checkbox("Loaded Only", &_onlyShowLoaded);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(240.0f);
		ImGui::InputTextWithHint("##AssetSearch", "Filter by path or type...", _searchFilter, sizeof(_searchFilter));

		ImGui::Separator();

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;
		if (ImGui::BeginTable("AssetBrowserTable", 4, tableFlags)) {
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 90.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Source Path", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Cooked Path", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			std::string_view filter(_searchFilter);

			for (const auto& [handle, source] : sources) {
				auto assetIt = assets.find(handle);
				const bool isLoaded = (
					assetIt != assets.end() && 
					assetIt->second && 
					((assetIt->second->GetAssetState() == AssetState::Loaded) || (assetIt->second->GetAssetState() == AssetState::Ready)));

				if (_onlyShowLoaded && !isLoaded) continue;
				if (!filter.empty() && source.sourcePath.find(filter) == std::string::npos && source.type.find(filter) == std::string::npos) {
					continue;
				}

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				if (isLoaded) {
					ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.4f, 1.0f), "[Loaded]");
				}
				else {
					ImGui::TextDisabled("[On Disk]");
				}

				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted(source.type.c_str());

				ImGui::TableSetColumnIndex(2);
				ImGui::PushID(static_cast<int>(handle.index));
				ImGui::Selectable(source.sourcePath.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					ImGui::SetDragDropPayload("ASSET_HANDLE", &handle, sizeof(AssetHandle));
					ImGui::Text("Assign %s (%s)", source.sourcePath.c_str(), source.type.c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::PopID();

				ImGui::TableSetColumnIndex(3);
				ImGui::TextDisabled("%s", source.cookedPath.c_str());
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
}