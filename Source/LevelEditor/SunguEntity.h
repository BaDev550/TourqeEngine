#pragma once

#include <Engine/Scripting/GameObject.h>
#include <Runtime/Definitions/Logger.h>

#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/World/Components/SpriteComponent.h>

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

		Texture2D* boxtexture = assetSystem->Get<Texture2D>("sungu.jpg");

		auto* spriteComponent = _owningScene->AddComponent<SpriteComponent>(_handle);
		spriteComponent->TextureAssetHandle = boxtexture->GetAssetHandle();
		spriteComponent->Visible = true;
	}

	virtual void OnUpdate() final {
		if (GetPosition().x > 5.0f) {
			SetPosition(Runtime::Math::Vec3(-5.0f, 0.0f, 0.0f));
		}
		SetPosition(GetPosition() + Runtime::Math::Vec3(0.01f, 0.0f, 0.0f));
	}

	virtual void OnDestroy() final {

	}
};