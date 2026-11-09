#include "GameObject.h"
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/ECS/Scene.h>

#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/IdenityComponent.h>

namespace Tourqe::Engine {
	const std::string& GameObject::GetName() const { return GetOwningScene()->FindComponent<IdenityComponent>(_handle)->name; }

	[[nodiscard]] Runtime::Math::Vec3 GameObject::GetPosition() const { return GetOwningScene()->FindComponent<TransformComponent>(_handle)->position; }
	[[nodiscard]] Runtime::Math::Vec3 GameObject::GetRotation() const { return GetOwningScene()->FindComponent<TransformComponent>(_handle)->GetEulerRotation(); }
	[[nodiscard]] Runtime::Math::Vec3 GameObject::GetScale() const { return GetOwningScene()->FindComponent<TransformComponent>(_handle)->scale; }

	void GameObject::SetPosition(const Runtime::Math::Vec3& position) {
		GetOwningScene()->FindComponent<TransformComponent>(_handle)->position = position;
	}

	void GameObject::SetRotation(const Runtime::Math::Vec3& rotation) {
		GetOwningScene()->FindComponent<TransformComponent>(_handle)->rotation = rotation;
	}

	void GameObject::SetScale(const Runtime::Math::Vec3& scale) {
		GetOwningScene()->FindComponent<TransformComponent>(_handle)->scale = scale;
	}

	Scene* GameObject::GetOwningScene() const {
		return _owningScene;
	}

	[[nodiscard]] EntityHandle GameObject::GetEntityHandle() const {
		return _handle;
	}

	void GameObject::OnInitialized(Scene* scene) {
		if (!scene) return;

		_owningScene = scene;
		_handle = GetOwningScene()->CreateEntity();

		IdenityComponent* ic = GetOwningScene()->AddComponent<IdenityComponent>(_handle);
		TransformComponent* tc = GetOwningScene()->AddComponent<TransformComponent>(_handle);
		ic->name = GetClassName();
		ic->uuid = UUID();
	}
}