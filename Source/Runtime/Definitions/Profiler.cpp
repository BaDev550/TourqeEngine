#include "Profiler.h"

namespace Runtime {
	Profiler& Profiler::Get() { 
		static Profiler instance;
		return instance;
	}

	void Profiler::Record(const std::string& name, double durationMs) {
		_timings[name] = durationMs;
	}

	const std::unordered_map<std::string, double>& Profiler::GetTimings() const { return _timings; }
}