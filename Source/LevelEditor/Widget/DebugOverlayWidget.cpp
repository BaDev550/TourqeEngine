#include "DebugOverlayWidget.h"

#include <Engine/Core/Engine.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/Systems/ScriptingSystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <Runtime/Definitions/Profiler.h>
#include <Widget/PropertyDrawer.h>
#include <Editor/EditorContext.h>

#include <imgui.h>

using namespace Tourqe::Engine;
using namespace Tourqe::LevelEditor;

void DebugOverlayWidget::Draw() {
	auto& ctx = EditorContext::Get();
	if (!ctx.showProfiler) return;

	auto* window = Engine::Get()->GetSubsystem<WindowSubsystem>()->GetWindow();
	auto* renderSystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();

	ImGui::SetNextWindowPos(ImVec2(290.0f, ImGui::GetFrameHeight() + 10.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320.0f, 240.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.75f);

	if (ImGui::Begin("Performance & Profiler", &ctx.showProfiler)) {
		f32 deltaTime = Engine::Get()->GetTime().GetDeltaTime();
		f32 frameMs = deltaTime * 1000.0f;
		f32 fps = (deltaTime > 0.00001f) ? (1.0f / deltaTime) : 0.0f;

		static float s_frameTimes[120] = {};
		static int s_offset = 0;
		s_frameTimes[s_offset] = frameMs;
		s_offset = (s_offset + 1) % 120;

		ImGui::Text("FPS: %.1f (%.2f ms)", fps, frameMs);
		ImGui::Text("Viewport: %ux%u", window->GetWidth(), window->GetHeight());
		if (renderSystem) {
			ImGui::Text("GPU Frame Signal: %llu", renderSystem->GetSignalValue());
		}

		ImGui::PlotLines("##FrameTimeGraph", s_frameTimes, 120, s_offset, "Frame Time (ms)", 0.0f, 33.3f, ImVec2(-1.0f, 50.0f));

		ImGui::Separator();
		ImGui::Text("CPU Profiler Scopes:");

		const auto& timings = Profiler::Get().GetTimings();
		if (timings.empty()) {
			ImGui::TextDisabled("No active profiler scopes.");
		}
		else {
			for (const auto& [name, ms] : timings) {
				float fraction = (frameMs > 0.001f) ? static_cast<float>(ms / frameMs) : 0.0f;
				char overlay[64];
				snprintf(overlay, sizeof(overlay), "%.2f ms", ms);
				ImGui::Text("%-22s", name.c_str());
				ImGui::SameLine();
				ImGui::ProgressBar(fraction, ImVec2(-1.0f, 14.0f), overlay);
			}
		}
	}
	ImGui::End();
}