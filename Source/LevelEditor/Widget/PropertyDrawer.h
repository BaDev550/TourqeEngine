#include "DebugOverlayWidget.h"
#include <Engine/Core/Engine.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <Runtime/Definitions/Profiler.h>
#include <Runtime/Reflection/Type.h>
#include <Runtime/Reflection/Property.h>

#include <imgui.h>
#include <string>

using namespace Tourqe::Engine;

namespace {
	void DrawReflectedObject(const Runtime::Reflection::Type* type, void* instance) {
		using namespace Runtime::Reflection;

		if (!type || !instance) return;

		ImGui::PushID(instance);
		if (ImGui::TreeNodeEx(type->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			for (const Property& prop : type->GetProperties()) {
				const bool canEdit = prop.HasFlag(PropertyFlags::Edit);
				const bool readOnly = prop.HasFlag(PropertyFlags::ReadOnly);

				if (!canEdit && !readOnly)
					continue;

				float step = 0.1f;
				if (const std::string* stepMeta = prop.GetMeta("Step")) {
					try { step = std::stof(*stepMeta); }
					catch (...) {}
				}

				if (readOnly) ImGui::BeginDisabled();

				const char* label = prop.GetName().c_str();
				const std::type_index propType = prop.GetType();

				if (propType == typeid(Runtime::Math::Vec3)) {
					auto* val = prop.GetPtr<Runtime::Math::Vec3>(instance);
					if (prop.HasFlag(PropertyFlags::Color)) {
						ImGui::ColorEdit3(label, &val->x);
					}
					else {
						ImGui::DragFloat3(label, &val->x, step);
					}
				}
				else if (propType == typeid(f32)) {
					ImGui::DragFloat(label, prop.GetPtr<f32>(instance), step);
				}
				else if (propType == typeid(i32)) {
					ImGui::DragInt(label, prop.GetPtr<i32>(instance), 1.0f);
				}
				else if (propType == typeid(u32)) {
					ImGui::DragScalar(label, ImGuiDataType_U32, prop.GetPtr<u32>(instance), 1.0f);
				}
				else if (propType == typeid(bool)) {
					ImGui::Checkbox(label, prop.GetPtr<bool>(instance));
				}
				else if (propType == typeid(b8)) {
					auto* val = prop.GetPtr<b8>(instance);
					bool b = (*val != 0);
					if (ImGui::Checkbox(label, &b)) *val = b;
				}
				else if (propType == typeid(std::string)) {
					auto* val = prop.GetPtr<std::string>(instance);
					char buffer[256] = {};
					strncpy_s(buffer, val->c_str(), sizeof(buffer) - 1);
					if (ImGui::InputText(label, buffer, sizeof(buffer))) {
						*val = buffer;
					}
				}
				else if (propType == typeid(Runtime::UUID)) {
					Runtime::UUID* uuid = prop.GetPtr<UUID>(instance);
					ImGui::Text("UUID: %s", uuid->Str().c_str());
				}
				else {
					ImGui::TextDisabled("UNKNOWNED TYPE: %s (%zu bytes)", label, prop.GetSize());
				}

				if (const std::string* category = prop.GetMeta("Category")) {
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
						ImGui::SetTooltip("Category: %s", category->c_str());
					}
				}

				if (readOnly) ImGui::EndDisabled();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	template<typename T>
	void DrawComponent(ReflectionSystem* reflection, T* component) {
		if (!reflection || !component) return;
		const Runtime::Reflection::Type* type = reflection->GetType(typeid(T));
		DrawReflectedObject(type, component);
	}
}