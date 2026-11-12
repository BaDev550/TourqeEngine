#include "Logger.h"
#include <mutex>
#include <chrono>
#include <print>

static std::vector<LogEntry> s_LogBuffer;
static std::mutex s_LogMutex;

const std::vector<LogEntry>& Logger::GetEntries() {
	std::lock_guard<std::mutex> lock(s_LogMutex);
	return s_LogBuffer;
}

void Logger::Clear() {
	std::lock_guard<std::mutex> lock(s_LogMutex);
	s_LogBuffer.clear();
}

void Logger::Print(LogLevel level, std::string_view category, std::string_view msg) {
	const auto now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	const auto time = std::chrono::floor<std::chrono::seconds>(now);
	std::string timeStr = std::format("{:%H:%M:%S}", time);

	{
		std::lock_guard<std::mutex> lock(s_LogMutex);
		s_LogBuffer.push_back({ level, std::string(category), std::string(msg), timeStr });

		if (s_LogBuffer.size() > 1000) {
			s_LogBuffer.erase(s_LogBuffer.begin());
		}
	}

	const char* name = "Info";
	const char* color = "\033[90m";

	switch (level) {
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
	}

	std::println("[{}][{}][{}{}\033[0m]: {}", timeStr, category, color, name, msg);
}