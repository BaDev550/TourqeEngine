#pragma once

#include <Runtime/Definitions/Types.h>

namespace Runtime {
	class Path final {
	public:
		Path() = default;
		Path(const std::string& path) : _path(path) {}
		Path(const char* path) : _path(path) {}
		Path(const Path& other) : _path(other._path) {}
		Path& operator=(const Path& other) { _path = other._path; return *this; }
		Path(Path&& other) noexcept : _path(std::move(other._path)) {}
		Path& operator=(Path&& other) noexcept { _path = std::move(other._path); return *this; }
		
		Path& operator=(const std::string& path) { _path = path; return *this; }
		Path& operator/(const Path& other) { _path = std::format("{}/{}", _path, other._path); return *this; }
		Path& operator/(const std::string& other) { _path = std::format("{}/{}", _path, other); return *this; }
		
		Path& ReplaceExtension(const std::string_view ext) {
			usize pos = _path.find_last_of(".");
			if (pos != std::string::npos) { _path.replace(pos, (_path.length() - pos), ext); }
			return *this;
		}

		std::string GetExtension() {
			usize pos = _path.find_last_of(".");
			if (pos != std::string::npos) { return _path.substr(0, (_path.length() - pos)); }
			return "";
		}
		std::string GetParentDirectory() const {
			usize pos = _path.find_last_of("/\\");
			if (pos != std::string::npos) { return _path.substr(0, pos); }
			return "";
		}

		[[nodiscard]] std::string GetPathStr() const { return _path; }
		[[nodiscard]] const char* GetPathCStr() const { return _path.c_str(); }

		bool operator==(const Path& other) const { return _path == other._path; }
		bool operator!=(const Path& other) const { return _path != other._path; }
		operator bool() const { return !_path.empty(); }
		operator std::string() const { return _path; }
	private:
		std::string _path = "\0";
		u32 _cachedParentDirStrPos = ~0ull; //mybe?
		u32 _cachedExtensionStrPos = ~0ull;
	};
}