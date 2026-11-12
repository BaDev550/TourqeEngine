#pragma once

#include <Runtime/Containers/UUID.h>
#include <Runtime/Definitions/Logger.h>
#include <chrono>
#include <list>
#include <string_view>

namespace Runtime {
	class RUNTIME_API Profiler final {
	public:
		static Profiler& Get();

		void Record(const std::string& name, double durationMs);
		const std::unordered_map<std::string, double>& GetTimings() const;
	private:
		std::unordered_map<std::string, double> _timings;
	};

	class ScopedTimer {
	public:
		ScopedTimer(const std::string& name) : _name(name), _start(std::chrono::high_resolution_clock::now()) {}

		~ScopedTimer() {
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - _start;
			Profiler::Get().Record(_name, elapsed.count());
		}

	private:
		std::string _name;
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	};
}
#define BEGIN_PROFILE_SCOPE(name) ::Runtime::ScopedTimer profiler_timer_##__LINE__(name)