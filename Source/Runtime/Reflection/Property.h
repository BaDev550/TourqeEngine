#pragma once

#include <Runtime/Reflection/PropertyFlags.h>
#include <Runtime/Definitions/Types.h>
#include <unordered_map>
#include <typeindex>

namespace Runtime::Reflection {
	class Property final {
	public:
		Property(const std::string& name, std::type_index type, usize offset, usize size, PropertyFlags flags)
			: _name(std::move(name)), _type(type), _offset(offset), _size(size), _flags(flags) {}

		[[nodiscard]] const std::string& GetName() const { return _name; }
		[[nodiscard]] std::type_index GetType() const { return _type; }
		[[nodiscard]] usize GetOffset() const { return _offset; }
		[[nodiscard]] usize GetSize() const { return _size; }
		[[nodiscard]] PropertyFlags GetFlags() const { return _flags; }
		[[nodiscard]] bool HasFlag(PropertyFlags flag) const { return (_flags & flag) != PropertyFlags::None; }

		void SetMeta(std::string key, std::string value) { _metadata[std::move(key)] = std::move(value); }

		[[nodiscard]] bool HasMeta(const std::string& key) const { return _metadata.find(key) != _metadata.end(); }
		[[nodiscard]] const std::string* GetMeta(const std::string& key) const {
			auto it = _metadata.find(key);
			return it != _metadata.end() ? &it->second : nullptr;
		}

		[[nodiscard]] void* GetRawPtr(void* instance) const { return static_cast<u8*>(instance) + _offset; }
		[[nodiscard]] const void* GetRawPtr(const void* instance) const { return static_cast<const u8*>(instance) + _offset; }

		template<typename T>
		[[nodiscard]] T* GetPtr(void* instance) const {
			if (_type != typeid(T)) return nullptr;
			return reinterpret_cast<T*>(GetRawPtr(instance));
		}

		template<typename T>
		[[nodiscard]] const T* GetPtr(const void* instance) const {
			if (_type != typeid(T)) return nullptr;
			return reinterpret_cast<const T*>(GetRawPtr(instance));
		}
	private:
		std::string _name;
		std::type_index _type;
		usize _offset;
		usize _size;
		PropertyFlags _flags;
		std::unordered_map<std::string, std::string> _metadata;
	};
}