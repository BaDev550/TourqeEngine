#pragma once

#include <Runtime/Definitions/Types.h>

namespace Runtime {
	class Path final {
	public:
		Path() = default;
		Path(const std::string& path) : _path(Normalize(path)) {}
		Path(const char* path) : _path(Normalize(path ? path : "")) {}
		Path(const Path& other) = default;
		Path& operator=(const Path& other) = default;
		Path(Path&& other) noexcept = default;
		Path& operator=(Path&& other) noexcept = default;

		Path& operator=(const std::string& path) { _path = Normalize(path); return *this; }
		Path operator/(const Path& other) const { return Join(_path, other._path); }
		Path operator/(const std::string& other) const { return Join(_path, other); }

		Path& ReplaceExtension(const std::string_view ext) {
			usize slashPos = _path.find_last_of("/\\");
			usize dotPos = _path.find_last_of('.');
			if (dotPos != std::string::npos && (slashPos == std::string::npos || dotPos > slashPos)) {
				if (!ext.empty() && ext.front() != '.') {
					_path.replace(dotPos + 1, _path.length() - (dotPos + 1), ext);
				}
				else {
					_path.replace(dotPos, _path.length() - dotPos, ext);
				}
			}
			return *this;
		}

		std::string GetExtension() {
			usize pos = _path.find_last_of(".");
			if (pos != std::string::npos) { return _path.substr(pos); }
			return "";
		}

		std::string GetParentDirectory() const {
			usize pos = _path.find_last_of("/\\");
			if (pos != std::string::npos) { return _path.substr(0, pos); }
			return "";
		}

		std::string GetFilename() const {
			usize pos = _path.find_last_of("/\\");
			if (pos != std::string::npos) { return _path.substr(pos + 1); }
			return _path;
		}

		[[nodiscard]] Path Relative(const Path& base) const {
			std::string baseStr = base._path;
			while (!baseStr.empty() && baseStr.back() == '/') {
				baseStr.pop_back();
			}
			if (baseStr.empty()) {
				return *this;
			}

			if (_path.size() > baseStr.size() && StartsWithCaseInsensitive(_path, baseStr) && _path[baseStr.size()] == '/') {
				return Path(_path.substr(baseStr.size() + 1));
			}

			std::string marker = "/" + baseStr + "/";
			usize pos = FindCaseInsensitive(_path, marker);
			if (pos != std::string::npos) {
				return Path(_path.substr(pos + marker.size()));
			}

			return *this;
		}

		[[nodiscard]] std::string GetPathStr() const { return _path; }
		[[nodiscard]] const char* GetPathCStr() const { return _path.c_str(); }
		[[nodiscard]] bool IsAbsolute() const { return (_path.size() >= 2 && _path[1] == ':') || (!_path.empty() && _path[0] == '/'); }

		bool operator==(const Path& other) const { return _path == other._path; }
		bool operator!=(const Path& other) const { return _path != other._path; }
		operator bool() const { return !_path.empty(); }
		operator std::string() const { return _path; }
	private:
		static std::string Normalize(std::string_view raw) {
			std::string res;
			res.reserve(raw.size());
			for (char c : raw) {
				char ch = (c == '\\') ? '/' : c;
				if (ch == '/' && !res.empty() && res.back() == '/') {
					continue;
				}
				res.push_back(ch);
			}
			return res;
		}

		static Path Join(std::string_view lhs, std::string_view rhs) {
			while (!lhs.empty() && (lhs.back() == '/' || lhs.back() == '\\')) lhs.remove_suffix(1);
			while (!rhs.empty() && (rhs.front() == '/' || rhs.front() == '\\')) rhs.remove_prefix(1);
			if (lhs.empty()) return Path(std::string(rhs));
			if (rhs.empty()) return Path(std::string(lhs));
			return Path(std::format("{}/{}", lhs, rhs));
		}

		static bool StartsWithCaseInsensitive(std::string_view str, std::string_view prefix) {
			if (str.size() < prefix.size()) return false;
			for (usize i = 0; i < prefix.size(); ++i) {
				if (std::tolower(static_cast<unsigned char>(str[i])) != std::tolower(static_cast<unsigned char>(prefix[i])))
					return false;
			}
			return true;
		}

		static usize FindCaseInsensitive(std::string_view str, std::string_view needle) {
			if (needle.empty() || str.size() < needle.size()) return std::string::npos;
			for (usize i = 0; i <= str.size() - needle.size(); ++i) {
				if (StartsWithCaseInsensitive(str.substr(i), needle))
					return i;
			}
			return std::string::npos;
		}

		std::string _path = "\0";
		u32 _cachedParentDirStrPos = ~0ull; //mybe?
		u32 _cachedExtensionStrPos = ~0ull;
	};
}