#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/Macros.h>
#include <Runtime/Math/Vector3.h>

#include <Engine/World/ECS/EntityDefinitions.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <Engine/World/Components/TransformComponent.h>

namespace Tourqe::Engine {
	class Scene;

	TCLASS();
	class ENGINE_API GameObject : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(GameObject);
	public:
		GameObject() = default;
		virtual ~GameObject() = default;

		virtual void OnCreate() {}
		virtual void OnUpdate() {}
		virtual void OnDestroy() {}

		const std::string& GetName() const;

		[[nodiscard]] Runtime::Math::Vec3 GetPosition() const;
		[[nodiscard]] Runtime::Math::Vec3 GetRotation() const;
		[[nodiscard]] Runtime::Math::Vec3 GetScale() const;

		void SetPosition(const Runtime::Math::Vec3& position);
		void SetRotation(const Runtime::Math::Vec3& rotation);
		void SetScale(const Runtime::Math::Vec3& scale);

		virtual Scene* GetOwningScene() const;

		[[nodiscard]] EntityHandle GetEntityHandle() const;
	private:
		EntityHandle _handle;
		Scene* _owningScene;

		void OnInitialized(Scene* scene);

		friend class ScriptingSystem;
	};
}