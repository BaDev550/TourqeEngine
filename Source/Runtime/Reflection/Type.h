#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Definitions/Logger.h>
#include <Runtime/Memory/Memory.h>
#include <functional>
#include <typeindex>
#include <string>
#include <string_view>

namespace Runtime::Reflection {
	class Type final {
	public:
		Type() = default;
		virtual ~Type() = default;
		Type(const Type&) = delete;
		Type& operator=(const Type&) = delete;
		Type(Type&&) noexcept = default;
		Type& operator=(Type&&) noexcept = default;

		[[nodiscard]] std::type_index GetTypeIndex() const { return _type; }
		[[nodiscard]] std::type_index GetBaseTypeIndex() const { return _baseType; }

		const std::string& GetName() const { return _name; }
		usize GetSize() const { return _size; }
		usize GetAlignment() const { return _alignment; }

		void ConstructAt(void* mem) const {
			if (!_constructFunc) {
				Logger::Error("Type", "{} has No default constructor", _name);
				return;
			}
			_constructFunc(mem);
		}
		
		void DestructAt(void* mem) const {
			if (!CanDestruct()) {
				Logger::Error("Type", "{} has No default deconstructor", _name);
				return;
			}
			_destructFunc(mem);
		}

		void MoveAt(void* dst, void* src) const {
			if (!CanMove()) {
				Logger::Error("Type", "{} has no default move constructor", _name);
				return;
			}

			_moveFunc(dst, src);
		}

		void* Create() const {
			if (!CanConstruct()) {
				Logger::Error("Type", "{} has no default constructor, cannot be created", _name);
				return nullptr;
			}

			void* mem = Mem::Allocator::Allocate(_size, _alignment);
			ConstructAt(mem);

			return mem;
		}

		void Destroy(void* instance) const {
			if (!instance)
				return;

			DestructAt(instance);
			Mem::Allocator::Free(instance);
		}

		[[nodiscard]] inline b8 CanConstruct() const { return (bool)_constructFunc; }
		[[nodiscard]] inline b8 CanDestruct() const { return (bool)_destructFunc; }
		[[nodiscard]] inline b8 CanMove() const { return (bool)_moveFunc; }
	private:
		using ConstructFunc = std::function<void(void*)>;
		using DestructFunc = std::function<void(void*)>;
		using MoveFunc = std::function<void(void*, void*)>;

		std::string _name;
		usize _size;
		usize _alignment;
		bool _abstractClass;

		std::type_index _type = typeid(void);
		std::type_index _baseType = typeid(void);

		ConstructFunc _constructFunc;
		DestructFunc _destructFunc;
		MoveFunc _moveFunc;

		template<typename>
		friend class TypeBuilder;
	};
}