#pragma once
#include <Runtime/Definitions/Types.h>
#include <iostream>

#ifdef _WIN32 // TODO(0x): move this to CMake
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#endif

#define ASSETMANAGER_DEBUG 1
#define RHI_OBJECT_DECONSTRUCT_DEBUG 1

#define ASSET_MAGIC 0x41534554 // ASET
#define SHADER_MAGIC 0x53484452 // SHDR
#define TEXTURE2D_MAGIC 0x54585432 // TXT2

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x000000FF
#define COLOR_RED 0xFF0000FF
#define COLOR_GREEN 0x00FF00FF
#define COLOR_BLUE 0x0000FFFF

#define CACHE_DIR Runtime::Path("cache/")
#define COOKED_ASSET_DIR Runtime::Path("cooked/")
#define ASSET_EXTENSION ".casset"
#define ASSET_REGISTRY_PATH "assetReg.json"