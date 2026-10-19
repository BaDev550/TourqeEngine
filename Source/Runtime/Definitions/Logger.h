#pragma once

#include <Runtime/Definitions/Types.h>
#include <chrono>
#include <string>
#include <print>
#include <string_view>

enum class LogLevel : u8 {
	Info = 0,
	Warn,
	Error,
	Fatal
};

class Logger final {
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
private:
	static void Print(LogLevel level, std::string_view catagory, std::string_view msg) {
		const char* name = "Info";
		const char* color = "\033[90m";

		switch (level)
		{
		case LogLevel::Info:
			name = "Info";
			color = "\033[32m";
			break;
		case LogLevel::Warn:
			name = "Warn";
			color = "\033[33m";
			break;
		case LogLevel::Error:
			name = "Error";
			color = "\033[31m";
			break;
		case LogLevel::Fatal:
			name = "Fatal";
			color = "\033[35m";
			break;
		default:
			break;
		}

		const auto now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
		const auto time = std::chrono::floor<std::chrono::seconds>(now);
		std::println("[{:%H:%M:%S}][{}][{}{}\033[0m]: {}", time, catagory, color, name, msg);
	}
};
