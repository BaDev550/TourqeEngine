#pragma once

#include <Runtime/Definitions/Types.h>
#include <iostream>
#include <sstream>

namespace Runtime {
	static constexpr u64 InvalidHandleId = ~0ull;

	template<typename T>
	struct Handle {
		u64 index = InvalidHandleId;

		Handle& operator=(const Handle& h) { index = h.index; return *this; }
		b8 operator==(const Handle& h) const { return h.index == index; }
		b8 operator!=(const Handle& h) const { return h.index != index; }

		b8 IsValid() const { return index != InvalidHandleId; }
		explicit operator b8() const { return IsValid(); }
	};

	template<typename T>
	void to_json(nlohmann::json& j, const Handle<T>& h) { j = h.index; }

	template<typename T>
	void from_json(const nlohmann::json& j, Handle<T>& h) { h.index = j.get<u64>(); }
}

namespace std {
	template<typename T>
	struct hash<Runtime::Handle<T>> {
		size_t operator()(const Runtime::Handle<T>& handle) const {
			return hash<u64>()(static_cast<u64>(handle.index));
		}
	};
}