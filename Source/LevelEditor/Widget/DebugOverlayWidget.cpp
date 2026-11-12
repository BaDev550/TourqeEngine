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

#include <imgui.h>

using namespace Tourqe::Engine;

void DebugOverlayWidget::Draw() {
	auto* windowSystem = Engine::Get()->GetSubsystem<WindowSubsystem>();
	auto* window = windowSystem->GetWindow();

	ImVec2 viewportPos = ImVec2(0.0f, 0.0f);
	ImVec2 viewportSize = ImVec2(static_cast<float>(window->GetWidth()) / 2.5f, static_cast<float>(window->GetHeight()));

	ImGui::SetNextWindowPos(viewportPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(viewportSize, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoFocusOnAppearing;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin(GetClassName().data(), nullptr, flags);

	f32 deltaTime = Engine::Get()->GetTime().GetDeltaTime();
	f32 fps = 1.0f / deltaTime;
	ImGui::Text("FPS: %.1f (%.2f ms)", fps, 1000.0f / fps);
	ImGui::Text("Resolution: %dx%d", window->GetWidth(), window->GetHeight());

	ImGui::Separator();
	auto* renderSystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();
	if (renderSystem) {
		ImGui::Text("Frame Index: %llu", renderSystem->GetSignalValue());
	}

	ImGui::Separator();
	const auto& timings = Profiler::Get().GetTimings();
	if (timings.empty()) {
		ImGui::Text("  No scopes active yet.");
	}
	else {
		for (const auto& [name, ms] : timings) {
			ImGui::Text("  %-30s: %.2f ms", name.c_str(), ms);
		}
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("World Subsystem Inspector")) {
		ImGui::Indent();
		auto* worldSystem = Engine::Get()->GetSubsystem<WorldSubsystem>();
		auto* reflectionSystem = Engine::Get()->GetReflectionSystem();
		auto* scriptSystem = worldSystem->GetSystem<ScriptingSystem>();

		if (worldSystem && worldSystem->GetActiveScene()) {
			auto scene = worldSystem->GetActiveScene();

			worldSystem->GetActiveScene()->ForEach<TransformComponent, IdenityComponent>([&](EntityHandle entity, TransformComponent& transform, IdenityComponent& idenity) {
				if (ImGui::Selectable(idenity.name.c_str())) {
					_selectedEntity = entity;
				}
				});

			ImGui::Separator();
			if (_selectedEntity.IsValid()) {
				if (IdenityComponent* idenity = scene->FindComponent<IdenityComponent>(_selectedEntity)) {
					ImGui::Text("UUID: %s", idenity->uuid.Str().c_str());
					DrawComponent(reflectionSystem, idenity);
				}

				if (TransformComponent* transform = scene->FindComponent<TransformComponent>(_selectedEntity)) {
					DrawComponent(reflectionSystem, transform);
				}

				if (scene->HasComponent<SpriteComponent>(_selectedEntity)) {
					SpriteComponent* sprite = scene->FindComponent<SpriteComponent>(_selectedEntity);
					DrawComponent(reflectionSystem, sprite);
				}

				if (scriptSystem->IsScriptable(_selectedEntity)) {
					if (GameObject* gameObject = scriptSystem->GetGameObject(_selectedEntity)) {
						const Runtime::Reflection::Type* scriptType = reflectionSystem->GetType(std::string(gameObject->GetClassName()));
						DrawReflectedObject(scriptType, gameObject);
					}
				}
			}
		}
		else {
			ImGui::Text("Active Scene: None");
		}
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Asset Subsystem Inspector")) {
		ImGui::Indent();
		auto assetSystem = Engine::Get()->GetSubsystem<AssetSubsystem>();

		if (assetSystem) {
			const auto& assets = assetSystem->GetAssets();
			const auto& sources = assetSystem->GetAssetSources();

			ImGui::Text("Loaded Assets Count:  %zu", assets.size());
			ImGui::Separator();

			if (ImGui::BeginChild("AssetInspectorScroll", ImVec2((viewportSize.x - 50.0f), 200), true)) {
				for (const auto& [handle, source] : sources) {
					ImGui::Text("Type: %s", source.type.c_str());
					ImGui::TextWrapped("Source Path: %s", source.sourcePath.c_str());
					ImGui::TextWrapped("Cooked Path: %s", source.cookedPath.c_str());

					auto assetIt = assets.find(handle);
					if (assetIt != assets.end() && assetIt->second) {
						AssetState state = assetIt->second->GetAssetState();
						const char* stateStr = (state == AssetState::Loaded) ? "Loaded" : "InDisk/Loading";
						ImGui::Text("Status: [%s]", stateStr);
					}
					else {
						ImGui::Text("Status: [Not Resident in Memory]");
					}
					ImGui::Separator();
				}
			}
			ImGui::EndChild();
		}
		else {
			ImGui::Text("AssetSubsystem unavailable");
		}
		ImGui::Unindent();
	}

	ImGui::End();
	ImGui::PopStyleVar();
}