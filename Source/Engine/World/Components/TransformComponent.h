#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Runtime/Math/Vector3.h>
#include <Runtime/Math/Quat.h>
#include <Runtime/Math/Mat4.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Tourqe::Engine {
	using namespace Runtime;

	TCLASS();
	class TransformComponent : public BaseComponent {
		GENERATE_CLASS(TransformComponent);
	public:
		TPROP(Edit, Transient, Step=0.1f);
		Math::Vec3 position = Math::Vec3(1.0f);

		TPROP(Edit, Transient, Step = 0.1f);
		Math::Quat rotation = Math::Quat();

		TPROP(Edit, Transient, Step = 0.1f);
		Math::Vec3 scale = Math::Vec3(1.0f);

		Math::Mat4 world = Math::Mat4();

		TransformComponent(const Math::Vec3& pos, const Math::Quat& rot, const Math::Vec3& scl) : position(pos), rotation(rot), scale(scl) {}
		TransformComponent(const Math::Vec3& pos, const Math::Vec3& rot, const Math::Vec3& scl) : position(pos), rotation(rot), scale(scl) {}
		TransformComponent() : position({ 0.0f }), rotation({ 0.0f, 0.0f, 0.0f, 1.0f }), scale({ 1.0f }) {}

		void SetRotation(const Math::Vec3& rot) {
			rotationEuler = rot;
			rotation = rotation.FromEuler(rotationEuler);
		}

		[[nodiscard]] const Math::Vec3 GetEulerRotation() const { return rotationEuler; }
	private:
		Math::Vec3 rotationEuler;
	};
}