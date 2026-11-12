#pragma once

#include <Engine/Scripting/GameObject.h>
#include <Runtime/Definitions/Logger.h>

#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
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

		_playerSprite = assetSystem->Get<Texture2D>("Assets/Dungeon/priest1_v1_1.png");

		auto* spriteComponent = GetOwningScene()->AddComponent<SpriteComponent>(GetEntityHandle());
		spriteComponent->TextureAssetHandle = _playerSprite->GetAssetHandle();
		spriteComponent->Visible = true;
		spriteComponent->zOrder = 100;

		SetScale(Runtime::Math::Vec3(5.0f));
	}

	virtual void OnUpdate() final {
		auto* inputSystem = Engine::Get()->GetSubsystem<InputSubsystem>();
		auto* graphicsSystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();

		if (inputSystem) {
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::W)) {
				SetPosition(
					GetPosition() + 
					Runtime::Math::Vec3(0.0f, 1.0f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime() * _speed
				);
			}
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::S)) {
				SetPosition(
					GetPosition() +
					Runtime::Math::Vec3(0.0f, -1.0f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime() * _speed
				);
			}
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::D)) {
				SetPosition(
					GetPosition() +
					Runtime::Math::Vec3(1.0f, 0.0f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime() * _speed
				);
				_direction = 1.0f;
			}
			if (inputSystem->IsKeyPressed(Runtime::KeyCode::A)) {
				SetPosition(
					GetPosition() +
					Runtime::Math::Vec3(-1.0f, 0.0f, 0.0f) * Engine::Get()->GetTime().GetDeltaTime() * _speed
				);
				_direction = -1.0f;
			}
		}
		SetRotation(Runtime::Math::Vec3(0.0f, _direction == -1.0f ? 180.0f : 0.0f , 0.0f));

		if (_playerSprite->GetAssetState() == AssetState::Ready && _id == ImTextureID_Invalid) {
			_id = graphicsSystem->GetImGuiContext()->AddTexture(_playerSprite->_image);
		}
	}

	virtual void OnDestroy() final {

	}
public:
	TPROP(Edit, Category="Movement", Step=0.1f)
	f32 _speed = 15.0f;

	TPROP(ReadOnly, Category = "Movement")
	f32 _direction = 1.0f;

	TPROP(ReadOnly)
	ImTextureID _id = ImTextureID_Invalid;

	Texture2D* _playerSprite;
};