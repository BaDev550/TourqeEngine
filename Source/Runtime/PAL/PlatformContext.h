#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Containers/Result.h>
#include <Runtime/PAL/Window/Window.h>
#include <Runtime/IO/Path.h>
#include <vector>

namespace Runtime::PAL{
	enum class Platform : u8 {
		Windows,
		Linux
	};

	class PlatformContext {
	public:
		virtual ~PlatformContext() = default;
		
		[[nodiscard]] virtual const Platform GetPlatform() const = 0;

		virtual Window* CreateNewWindow(const WindowDesc& desc) = 0;

		virtual void ImGuiCreateContext(void* windowHandle) = 0;
		virtual void ImGuiDestroyContext() = 0;
		virtual void ImGuiNewFrame() = 0;

		virtual Result WriteToFile(const Path& filePath, char* data, usize dataSize, usize dataOffset = 0) = 0;
		virtual Result WriteToFile(const Path& filePath, const std::vector<u8>& data, usize dataOffset = 0) = 0;

		virtual std::vector<u8> ReadFromFile(const Path& filePath) = 0;
		virtual Result MapFile(const Path& filePath, std::vector<u8>& mappedBuffer) = 0;

		virtual Result CreateF(const Path& filePath) = 0;
		virtual Result DeleteF(const Path& filePath) = 0;
		virtual Result CreateD(const Path& directoryPath) = 0;
		virtual Result DeleteD(const Path& directoryPath) = 0;

		virtual b8 FileExists(const Path& filePath) = 0;
		virtual b8 DirectoryExists(const Path& directoryPath) = 0;
	};
	PlatformContext* CreatePlatformContext();
}