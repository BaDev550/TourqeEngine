#pragma once

#include <Runtime/Reflection/Type.h>

namespace Runtime::Reflection {
	template<typename T>
	class TypeBuilder final {
	public:
		static TypeBuilder ForType(std::string name) {
			TypeBuilder builder = {};
			builder._type._name = std::move(name);
			builder._type._type = typeid(T);
			builder._type._size = sizeof(T);
			builder._type._alignment = alignof(T);
			builder._type._abstractClass = std::is_abstract_v<T>;

			if constexpr (!std::is_abstract_v<T>) {
				builder._type._destructFunc = [](void* mem) {
					static_cast<T*>(mem)->~T();
					};

				if constexpr (std::is_default_constructible_v<T>) {
					builder._type._constructFunc = [](void* mem) {
						::new (mem) T();
						};
				}

				if constexpr (std::is_move_constructible_v<T>)
				{
					builder._type._moveFunc = [](void* pDestination, void* pSource) {
							::new (pDestination) T(std::move(*static_cast<T*>(pSource)));
						};
				}
			}
			return builder;
		}

		template<typename TBase>
		TypeBuilder& Base() {
			_type._baseType = typeid(TBase);
			return *this;
		}

		template<auto MemberPtr>
		TypeBuilder& Property(std::string name, PropertyFlags flags = PropertyFlags::Edit | PropertyFlags::Serialize) {
			using MemberT = std::remove_cvref_t<decltype(std::declval<T>().*MemberPtr)>;
			const usize offset = reinterpret_cast<usize>(&reinterpret_cast<char const volatile&>(((static_cast<T*>(nullptr))->*MemberPtr)));
			_type._properties.emplace_back(
				std::move(name),
				std::type_index(typeid(MemberT)),
				offset,
				sizeof(MemberT),
				flags
			);
			return *this;
		}

		TypeBuilder& Meta(std::string key, std::string value) {
			if (!_type._properties.empty()) {
				_type._properties.back().SetMeta(std::move(key), std::move(value));
			}
			return *this;
		}

		Type Build() { return std::move(_type); }
	private:
		Type _type;
	};
}