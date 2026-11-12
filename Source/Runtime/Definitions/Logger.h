#pragma once

#include <Runtime/Definitions/Types.h>
#include <chrono>
#include <string>
#include <print>
#include <string_view>
#include <vector> 
#include <mutex>

enum class LogLevel : u8 {
	Info = 0,
	Warn,
	Error,
	Fatal
};

struct LogEntry {
	LogLevel level;
	std::string category;
	std::string message;
	std::string timeStr;
};

class RUNTIME_API Logger final {
public:
	template<typename... Args>
	static void Info(std::string_view catagory, std::format_string<Args...> fmt, Args&&... args) {
		std::string msg = std::format(fmt, std::forward<Args>(args)...);
		Print(LogLevel::Info, catagory, msg);
	}
	template<typename... Args>
	static void Warn(std::string_view catagory, std::format_string<Args...> fmt, Args&&... args) {
		std::string msg = std::format(fmt, std::forward<Args>(args)...);
		Print(LogLevel::Warn, catagory, msg);
	}
	template<typename... Args>
	static void Error(std::string_view catagory, std::format_string<Args...> fmt, Args&&... args) {
		std::string msg = std::format(fmt, std::forward<Args>(args)...);
		Print(LogLevel::Error, catagory, msg);
	}
	template<typename... Args>
	static void Fatal(std::string_view catagory, std::format_string<Args...> fmt, Args&&... args) {
		std::string msg = std::format(fmt, std::forward<Args>(args)...);
		Print(LogLevel::Fatal, catagory, msg);
	}

	template<typename... Args>
	static void Assert(bool x, std::string_view catagory, std::format_string<Args...> fmt, Args&&... args) {
		if (x) return;

		std::string msg = std::format(fmt, std::forward<Args>(args)...);
		Print(LogLevel::Fatal, catagory, msg);
		std::abort();
	}

	static const std::vector<LogEntry>& GetEntries();
	static void Clear();
private:
	static void Print(LogLevel level, std::string_view category, std::string_view msg);
};
