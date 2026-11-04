#pragma once
#include <Runtime/Definitions/Types.h>
#include <iostream>
#include <random>
#include <sstream>

#include <nlohmann/json.hpp>

namespace Runtime {
	static constexpr u64 InvalidUUID = 0;

	struct UUID {
	public:
		UUID() {
			std::random_device rd;
			std::mt19937_64 gen(rd());
			std::uniform_int_distribution<u64> dis;
			uuid = dis(gen);
		}
		UUID(u64 uuid) : uuid(uuid) {}
		UUID(std::string_view uuidStr) { uuid = std::hash<std::string>{}(uuidStr.data()); }
		
		b8 IsValid() const { return uuid != InvalidUUID; }

		b8 operator==(const UUID& id) const { return id.uuid == uuid; }
		b8 operator!=(const UUID& id) const { return id.uuid != uuid; }
		explicit operator b8() const { return IsValid(); }
		operator u64() const { return uuid; }

		std::string Str() const { return std::to_string(uuid); }
	private:
		u64 uuid = InvalidUUID;
	};
}

namespace nlohmann {
	inline void to_json(json& j, const Runtime::UUID& uuid) { j = static_cast<u64>(uuid); }
	inline void from_json(const json& j, Runtime::UUID& uuid) { uuid = j.get<u64>(); }
}

namespace std {
	template<>
	struct hash<Runtime::UUID> {
		size_t operator()(const Runtime::UUID& uuid) const {
			return hash<u64>()(static_cast<u64>(uuid));
		}
	};
}