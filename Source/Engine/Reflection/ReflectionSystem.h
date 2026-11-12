#pragma once

#include <Engine/Core/Core.h>
#include <Runtime/Reflection/Type.h>
#include <vector>
#include <functional>

namespace Tourqe::Engine {
	class ENGINE_API ReflectionSystem final {
	public:
		ReflectionSystem();
		~ReflectionSystem();
		ReflectionSystem(const ReflectionSystem&) = delete;
		ReflectionSystem& operator=(const ReflectionSystem&) = delete;
		ReflectionSystem(ReflectionSystem&&) noexcept = default;
		ReflectionSystem& operator=(ReflectionSystem&&) noexcept = default;

		template<typename TBase>
		void ForEachWithBase(std::function<void(const Runtime::Reflection::Type& type)> func) {
			for (auto& type : _types) {
				if (type.GetBaseTypeIndex() == typeid(TBase))
					func(type);
			}
		}

		Runtime::Reflection::Type* GetType(const std::string& name);
		Runtime::Reflection::Type* GetType(const std::type_index& type);

		template<typename T>
		Runtime::Reflection::Type* GetType() {
			return GetType(std::type_index(typeid(T)));
		}
	private:
		std::vector<Runtime::Reflection::Type> _types;
		std::unordered_map<std::type_index, Runtime::Reflection::Type*> _lookupTable;
		std::unordered_map<std::string, Runtime::Reflection::Type*> _nameLookupTable;
	};
}