#pragma once

#include <Runtime/Containers/UUID.h>
#include <Runtime/Definitions/Logger.h>
#include <chrono>
#include <list>
#include <string_view>

namespace Runtime {
	class Profiler final {
	public:
		static Profiler& Get() { // FIXME(0x): needs to moved into a engine class or a subsystem
			static Profiler instance;
			return instance;
		}

		void Record(const std::string& name, double durationMs) {
			_timings[name] = durationMs;
		}

		const std::unordered_map<std::string, double>& GetTimings() const { return _timings; }
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