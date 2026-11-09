#include "LevelEditor.h"

#include <Engine/Core/Engine.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/World/ECS/Scene.h>

#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/IdenityComponent.h>

#include <imgui.h>

using namespace Tourqe::Engine;
using namespace Runtime;

Runtime::Result LevelEditor::OnCreate(Engine* engine) {
    Subsystem::OnCreate(engine);
	auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();
	auto worldSystem = _engine->GetSubsystem<WorldSubsystem>();
	auto assetSystem = _engine->GetSubsystem<AssetSubsystem>();
	auto context = renderSystem->GetContext();

	//ImGui::SetCurrentContext(context->GetImGuiContext());
	Texture2D* boxtexture = assetSystem->Get<Texture2D>("warrior_bg.jpg");
	Texture2D* rocktexture = assetSystem->Get<Texture2D>("player_sprite.png");
	Texture2D* transparenttexture = assetSystem->Get<Texture2D>("Org.png");

	{
		EntityHandle entity1 = worldSystem->GetActiveScene()->CreateEntity();
		IdenityComponent* ic = worldSystem->GetActiveScene()->AddComponent<IdenityComponent>(entity1);
		TransformComponent* tc = worldSystem->GetActiveScene()->AddComponent<TransformComponent>(entity1);
		SpriteComponent* sc = worldSystem->GetActiveScene()->AddComponent<SpriteComponent>(entity1);
		ic->name = "BOX_ColorTest";
		ic->uuid = UUID();

		tc->position = Math::Vec3(0.0f, 0.0f, 0.0f);
		tc->rotation = Math::Vec3(0.0f, 0.0f, 0.0f);
		tc->scale = Math::Vec3(19.2f, 10.4f, 5.0f);

		tc->SetRotation(Math::Vec3(0.0f, 0.0f, 0.0f));

		sc->TextureAssetHandle = boxtexture->GetAssetHandle();
		sc->Visible = true;
	}
	{
		EntityHandle entity1 = worldSystem->GetActiveScene()->CreateEntity();
		IdenityComponent* ic = worldSystem->GetActiveScene()->AddComponent<IdenityComponent>(entity1);
		TransformComponent* tc = worldSystem->GetActiveScene()->AddComponent<TransformComponent>(entity1);
		SpriteComponent* sc = worldSystem->GetActiveScene()->AddComponent<SpriteComponent>(entity1);
		ic->name = "PIXELART_ColorTest";
		ic->uuid = UUID();

		tc->position = Math::Vec3(-2.5f, -3.2f, 0.0f);
		tc->rotation = Math::Vec3(0.0f, 0.0f, 0.0f);
		tc->scale = Math::Vec3(-2.0f, 2.0f, 2.0f);

		sc->TextureAssetHandle = rocktexture->GetAssetHandle();
		sc->Visible = true;
	}
	{
		EntityHandle entity1 = worldSystem->GetActiveScene()->CreateEntity();
		IdenityComponent* ic = worldSystem->GetActiveScene()->AddComponent<IdenityComponent>(entity1);
		TransformComponent* tc = worldSystem->GetActiveScene()->AddComponent<TransformComponent>(entity1);
		SpriteComponent* sc = worldSystem->GetActiveScene()->AddComponent<SpriteComponent>(entity1);
		ic->name = "PIXELART_TransparentTest";
		ic->uuid = UUID();

		tc->position = Math::Vec3(3.1f, -1.7f, 0.0f);
		tc->rotation = Math::Vec3(0.0f, 0.0f, 0.0f);
		tc->scale = Math::Vec3(5.0f, 5.0f, 5.0f);

		sc->TextureAssetHandle = transparenttexture->GetAssetHandle();
		sc->Visible = true;
	}
    return Runtime::Result();
}

void LevelEditor::OnUpdate() {
	auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();
	
	//widgetRenderPass->Submit([this](Runtime::RHI::CommandBuffer* cmd) {
	//	console.Draw();
	//	});
}

void LevelEditor::OnDestroy() {

}

void LevelEditor::GetDependencyGraph(DependencyGraph & graph) {
	graph.Require<GraphicsSubsystem>(DependencyOrder::After);
	graph.Require<WorldSubsystem>(DependencyOrder::After);
}

void Console::Draw() {
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Console")) {
		ImGui::End();
		return;
	}

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	for (const auto& log : _history) {
		ImGui::TextUnformatted(log.c_str());
	}
	ImGui::PopStyleVar();

	if (_scrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
		ImGui::SetScrollHereY(1.0f);
		_scrollToBottom = false;
	}

	ImGui::EndChild();
	ImGui::Separator();

	static char inputBuffer[256] = "";
	bool reclaimFocus = false;

	ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
	if (ImGui::InputText("Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), inputFlags)) {
		std::string command(inputBuffer);

		inputBuffer[0] = '\0';
		reclaimFocus = true;

		if (!command.empty()) {
			Log("> " + command);
			ProcessCommand(command);
		}
	}

	ImGui::SetItemDefaultFocus();
	if (reclaimFocus) {
		ImGui::SetKeyboardFocusHere(-1);
	}

	ImGui::End();
}

void Console::ProcessCommand(std::string_view command) {
	if (command == "clear") {
		_history.clear();
	}
	else if (command == "help") {
		Log("Available commands: clear, help, ping");
	}
	else if (command == "ping") {
		Log("pong!");
	}
	else if (command == "spawnObject") {
		auto* world = Tourqe::Engine::Engine::Get()->GetSubsystem<WorldSubsystem>();

		world->GetActiveScene()->CreateEntity();
	}
	else {
		Log("Unknown command: " + std::string(command));
	}
}

void Console::Log(std::string_view msg) {
	_history.emplace_back(msg);
	_scrollToBottom = true;
}
