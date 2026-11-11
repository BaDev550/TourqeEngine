#pragma once

#include <Engine/Scripting/GameObject.h>
#include <Runtime/Definitions/Logger.h>

#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/Input/InputSubsystem.h>

#include <Engine/World/ECS/Scene.h>
#include <Engine/Core/Engine.h>

using namespace Tourqe::Engine;

TCLASS();
class SunguEntity : public GameObject {
	GENERATE_CLASS(SunguEntity);
public:
	virtual void OnCreate() final {
		Logger::Info("Erturul", "SunguEntity created");
		auto assetSystem = Engine::Get()->GetSubsystem<AssetSubsystem>();

		Texture2D* boxtexture = assetSystem->Get<Texture2D>("player_sprite.png");

		auto* spriteComponent = GetOwningScene()->AddComponent<SpriteComponent>(GetEntityHandle());
		spriteComponent->TextureAssetHandle = boxtexture->GetAssetHandle();
		spriteComponent->Visible = true;
	}

	virtual void OnUpdate() final {
		auto* inputSystem = Engine::Get()->GetSubsystem<InputSubsystem>();

		if (inputSystem) {
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::W)) {
				SetPosition(
					GetPosition() + 
					Runtime::Math::Vec3(0.0f, 0.6f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime()
				);
			}
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::S)) {
				SetPosition(
					GetPosition() +
					Runtime::Math::Vec3(0.0f, -0.6f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime()
				);
			}
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::D)) {
				SetPosition(
					GetPosition() +
					Runtime::Math::Vec3(0.6f, 0.0f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime()
				);
			}
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::A)) {
				SetPosition(
					GetPosition() +
					Runtime::Math::Vec3(-0.6f, 0.0f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime()
				);
			}
		}
	}

	virtual void OnDestroy() final {

	}
};