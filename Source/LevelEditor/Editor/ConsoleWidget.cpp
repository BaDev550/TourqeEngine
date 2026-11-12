#include "ConsoleWidget.h"
#include <Editor/EditorContext.h>
#include <Runtime/Definitions/Logger.h>
#include <imgui.h>

using namespace Tourqe::Engine;
using namespace Tourqe::LevelEditor;

void ConsolleWidget::Draw() {
	auto& ctx = EditorContext::Get();
	if (!ctx.showConsolle) return;

	if (ImGui::Begin("Console", &ctx.showConsolle)) {

		if (ImGui::Button("Clear")) {
			Logger::Clear();
		}
		ImGui::SameLine();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (const auto& entry : Logger::GetEntries()) {
			ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			if (entry.level == LogLevel::Warn)  color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
			if (entry.level == LogLevel::Error) color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
			if (entry.level == LogLevel::Fatal) color = ImVec4(0.8f, 0.2f, 0.8f, 1.0f);

			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::Text("[%s][%s]: %s", entry.timeStr.c_str(), entry.category.c_str(), entry.message.c_str());
			ImGui::PopStyleColor();
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
	}
	ImGui::End();
}