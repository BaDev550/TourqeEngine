#include "DebugOverlayWidget.h"

#include <Engine/Core/Engine.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Window/WindowSubsystem.h>

#include <imgui.h>

namespace Tourqe::Engine {
	void DebugOverlayWidget::Draw() {
		auto* renderSystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();
		auto* windowSystem = Engine::Get()->GetSubsystem<WindowSubsystem>();
		auto* window = windowSystem->GetWindow();

		ImVec2 windowSize = ImVec2(window->GetWidth(), window->GetHeight());
		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin(GetClassName().data(), nullptr, 
			ImGuiWindowFlags_NoDecoration | 
			ImGuiWindowFlags_NoInputs | 
			ImGuiWindowFlags_NoResize | 
			ImGuiWindowFlags_NoTitleBar
		);

		ImGui::Text("Test");

		ImGui::End();
	}
}