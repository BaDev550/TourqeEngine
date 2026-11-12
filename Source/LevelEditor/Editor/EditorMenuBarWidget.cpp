#include "EditorMenuBarWidget.h"
#include "EditorContext.h"
#include <Engine/Core/Engine.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <Engine/World/Components/TransformComponent.h>
#include <imgui.h>

using namespace Tourqe::Engine;
using namespace Tourqe::LevelEditor;

void EditorMenuBarWidget::Draw() {
	auto& ctx = EditorContext::Get();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
				ctx.selectedEntity = { 0xFFFFFFFF, 0 };
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				Engine::Get()->Shutdown("Requested by editor");
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("Scene Outliner", nullptr, &ctx.showHierarchy);
			ImGui::MenuItem("Inspector", nullptr, &ctx.showInspector);
			ImGui::MenuItem("Asset Browser", nullptr, &ctx.showAssetBrowser);
			ImGui::MenuItem("Performance & Stats", nullptr, &ctx.showProfiler);
			ImGui::MenuItem("Consolle", nullptr, &ctx.showConsolle);
			ImGui::EndMenu();
		}

		f32 dt = Engine::Get()->GetTime().GetDeltaTime();
		f32 fps = (dt > 0.00001f) ? (1.0f / dt) : 0.0f;
		char fpsText[64];
		snprintf(fpsText, sizeof(fpsText), "%.1f FPS (%.2f ms)", fps, dt * 1000.0f);

		float textWidth = ImGui::CalcTextSize(fpsText).x;
		ImGui::SameLine(ImGui::GetWindowWidth() - textWidth - 20.0f);
		ImGui::TextDisabled("%s", fpsText);

		ImGui::EndMainMenuBar();
	}
}