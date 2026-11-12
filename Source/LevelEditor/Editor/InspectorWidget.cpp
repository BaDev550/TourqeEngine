#include "InspectorWidget.h"
#include "EditorContext.h"
#include <Widget/PropertyDrawer.h>
#include <Engine/Core/Engine.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/Systems/ScriptingSystem.h>
#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <Engine/World/Components/PointLightComponent.h>
#include <imgui.h>

using namespace Tourqe::Engine;
using namespace Tourqe::LevelEditor;

void InspectorWidget::Draw() {
	auto& ctx = EditorContext::Get();
	if (!ctx.showInspector) return;

	auto* window = Engine::Get()->GetSubsystem<WindowSubsystem>()->GetWindow();
	float menuBarHeight = ImGui::GetFrameHeight();
	float winWidth = 340.0f;
	float winHeight = static_cast<float>(window->GetHeight()) - menuBarHeight;

	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(window->GetWidth()) - winWidth, menuBarHeight), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Inspector", &ctx.showInspector)) {
		auto* worldSystem = Engine::Get()->GetSubsystem<WorldSubsystem>();
		Scene* scene = worldSystem ? worldSystem->GetActiveScene() : nullptr;

		if (!scene || !ctx.selectedEntity.IsValid()) {
			ImGui::TextDisabled("Select an entity in the Scene Outliner to inspect its properties.");
			ImGui::End();
			return;
		}

		auto* reflectionSystem = Engine::Get()->GetReflectionSystem();
		auto* scriptSystem = worldSystem->GetSystem<ScriptingSystem>();

		if (IdenityComponent* idenity = scene->FindComponent<IdenityComponent>(ctx.selectedEntity)) {
			ImGui::TextDisabled("Entity ID: %u (Gen %u)", ctx.selectedEntity.index, ctx.selectedEntity.generation);
			ImGui::TextDisabled("UUID: %s", idenity->uuid.Str().c_str());
			DrawComponent(reflectionSystem, idenity);
			ImGui::Separator();
		}

		if (TransformComponent* transform = scene->FindComponent<TransformComponent>(ctx.selectedEntity)) {
			DrawComponent(reflectionSystem, transform);

			if (ImGui::Button("Reset Position")) transform->position = Runtime::Math::Vec3(0.0f);
			ImGui::SameLine();
			if (ImGui::Button("Reset Scale"))    transform->scale = Runtime::Math::Vec3(1.0f);
			ImGui::Separator();
		}

		if (scene->HasComponent<SpriteComponent>(ctx.selectedEntity)) {
			SpriteComponent* sprite = scene->FindComponent<SpriteComponent>(ctx.selectedEntity);
			DrawComponent(reflectionSystem, sprite);
			ImGui::Separator();
		}

		if (scene->HasComponent<PointLightComponent>(ctx.selectedEntity)) {
			PointLightComponent* light = scene->FindComponent<PointLightComponent>(ctx.selectedEntity);
			DrawComponent(reflectionSystem, light);
			ImGui::Separator();
		}

		if (scriptSystem && scriptSystem->IsScriptable(ctx.selectedEntity)) {
			if (GameObject* gameObject = scriptSystem->GetGameObject(ctx.selectedEntity)) {
				const Runtime::Reflection::Type* scriptType = reflectionSystem->GetType(std::string(gameObject->GetClassName()));
				ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "GO: %s", gameObject->GetClassName().data());
				DrawReflectedObject(scriptType, gameObject);
				ImGui::Separator();
			}
		}

		ImGui::Spacing();
		float buttonWidth = ImGui::GetContentRegionAvail().x;
		if (ImGui::Button("+ Add Component", ImVec2(buttonWidth, 28.0f))) {
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup")) {
			if (!scene->HasComponent<IdenityComponent>(ctx.selectedEntity)) {
				if (ImGui::MenuItem("Idenity Component")) {
					scene->AddComponent<IdenityComponent>(ctx.selectedEntity);
					ImGui::CloseCurrentPopup();
				}
			}
			if (!scene->HasComponent<TransformComponent>(ctx.selectedEntity)) {
				if (ImGui::MenuItem("Transform Component")) {
					scene->AddComponent<TransformComponent>(ctx.selectedEntity);
					ImGui::CloseCurrentPopup();
				}
			}
			if (!scene->HasComponent<SpriteComponent>(ctx.selectedEntity)) {
				if (ImGui::MenuItem("Sprite Component")) {
					scene->AddComponent<SpriteComponent>(ctx.selectedEntity);
					ImGui::CloseCurrentPopup();
				}
			}
			if (!scene->HasComponent<PointLightComponent>(ctx.selectedEntity)) {
				if (ImGui::MenuItem("Point Light Component")) {
					scene->AddComponent<PointLightComponent>(ctx.selectedEntity);
					ImGui::CloseCurrentPopup();
				}
			}
			else {
				ImGui::TextDisabled("All available components attached");
			}
			ImGui::EndPopup();
		} // TODO(0x): automate
	}
	ImGui::End();
}