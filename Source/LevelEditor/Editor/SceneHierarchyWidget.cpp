#include "SceneHierarchyWidget.h"
#include "EditorContext.h"
#include <Engine/Core/Engine.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/Systems/ScriptingSystem.h>
#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <imgui.h>
#include <string_view>

using namespace Tourqe::Engine;
using namespace Tourqe::LevelEditor;

void SceneHierarchyWidget::Draw() {
	auto& ctx = EditorContext::Get();
	if (!ctx.showHierarchy) return;

	auto* window = Engine::Get()->GetSubsystem<WindowSubsystem>()->GetWindow();
	float menuBarHeight = ImGui::GetFrameHeight();
	float winWidth = 280.0f;
	float winHeight = static_cast<float>(window->GetHeight()) * 0.65f - menuBarHeight;

	ImGui::SetNextWindowPos(ImVec2(0.0f, menuBarHeight), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Scene Outliner", &ctx.showHierarchy)) {
		auto* worldSystem = Engine::Get()->GetSubsystem<WorldSubsystem>();
		Scene* scene = worldSystem ? worldSystem->GetActiveScene() : nullptr;

		if (!scene) {
			ImGui::TextDisabled("No Active Scene");
			ImGui::End();
			return;
		}

		if (ImGui::Button("+ Add Entity")) {
			ctx.selectedEntity = scene->CreateEntity();
			scene->AddComponent<IdenityComponent>(ctx.selectedEntity);
			scene->AddComponent<TransformComponent>(ctx.selectedEntity);
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputTextWithHint("##HierarchySearch", "Search entities...", _searchFilter, sizeof(_searchFilter));

		ImGui::Separator();

		if (ImGui::BeginChild("EntityListScroll")) {
			auto* scriptSystem = worldSystem->GetSystem<ScriptingSystem>();
			std::string_view filterView(_searchFilter);

			scene->ForEach<TransformComponent, IdenityComponent>([&](EntityHandle entity, TransformComponent&, IdenityComponent& idenity) {
				if (!filterView.empty() && idenity.name.find(filterView) == std::string::npos) {
					return;
				}

				ImGui::PushID(static_cast<int>(entity.index));

				const bool isSelected = (ctx.selectedEntity == entity);
				const bool hasSprite = scene->HasComponent<SpriteComponent>(entity);
				const bool isScripted = scriptSystem && scriptSystem->IsScriptable(entity);

				std::string displayLabel = idenity.name.empty() ? "<Unnamed Entity>" : idenity.name;
				if (isScripted)      displayLabel = "[S] " + displayLabel;
				else if (hasSprite)  displayLabel = "[D] " + displayLabel;
				else                 displayLabel = "[E] " + displayLabel;

				if (ImGui::Selectable(displayLabel.c_str(), isSelected)) {
					ctx.selectedEntity = entity;
				}

				if (ImGui::BeginPopupContextItem("EntityContextMenu")) {
					ctx.selectedEntity = entity;
					ImGui::TextDisabled("UUID: %s", idenity.uuid.Str().c_str());
					ImGui::Separator();
					if (!hasSprite && ImGui::MenuItem("Add SpriteComponent")) {
						scene->AddComponent<SpriteComponent>(entity);
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
				});

			// Click empty space to deselect
			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
				ctx.selectedEntity = { 0xFFFFFFFF, 0 };
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}