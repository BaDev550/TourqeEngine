#include "ReflectionSystem.h"

#include <Engine/Core/Engine.h>
#include <Engine/Platform/PlatformSystem.h>

namespace Tourqe::Engine {
    ReflectionSystem::ReflectionSystem() {
		auto platform = Engine::Get()->GetPlatformSystem()->GetPlatform();
		using GenerateFn = void(*)(std::vector<Runtime::Reflection::Type>*);

		auto* generateManifest = reinterpret_cast<GenerateFn>(platform->GetMainModuleSymbol("GenerateModuleManifestation"));
		if (!generateManifest) {
			Logger::Error("ReflectionSubsystem", "Executable does not export 'GenerateModuleManifestation'");
			return;
		}

		generateManifest(&_types);

		for (auto& type : _types) {
            _lookupTable[type.GetTypeIndex()] = &type;
            _nameLookupTable[type.GetName()] = &type;

			Logger::Info("ReflectionSubsystem", "Type {} registered", type.GetName());
		}
    }

    ReflectionSystem::~ReflectionSystem() {
    
    }

	Runtime::Reflection::Type* ReflectionSystem::GetType(const std::string& name)
	{
		if (!_nameLookupTable.contains(name)) {
			Logger::Error("ReflectionSystem", "Requested type '{}' is not registered", name);
			return nullptr;
		}
		auto it = _nameLookupTable.find(name);
		return it->second;
	}

	Runtime::Reflection::Type* ReflectionSystem::GetType(const std::type_index& type)
	{
		if (!_lookupTable.contains(type)) {
			Logger::Error("ReflectionSystem", "Requested type '{}' is not registered", type.name());
			return nullptr;
		}
		auto it = _lookupTable.find(type);
		return it->second;
	}
}