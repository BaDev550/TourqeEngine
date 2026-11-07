#pragma once

#include <Runtime/PAL/PlatformContext.h>

namespace Runtime::PAL {
	class WindowsContext final : public PlatformContext {
	public:
		WindowsContext() = default;
		virtual ~WindowsContext() = default;
		
		[[nodiscard]] virtual const Platform GetPlatform() const final { return Platform::Windows; }

		virtual Window* CreateNewWindow(const WindowDesc& desc) final;

		virtual void ImGuiCreateContext(void* windowHandle) final;
		virtual void ImGuiDestroyContext() final;
		virtual void ImGuiNewFrame() final;

		virtual Result WriteToFile(const Path& filePath, char* data, usize dataSize, usize dataOffset = 0) final;
		virtual Result WriteToFile(const Path& filePath, const std::vector<u8>& data, usize dataOffset = 0) final;

		virtual std::vector<u8> ReadFromFile(const Path& filePath) final;
		virtual Result MapFile(const Path& filePath, std::vector<u8>& mappedBuffer) final;

		virtual Result CreateF(const Path& filePath) final;
		virtual Result DeleteF(const Path& filePath) final;
		virtual Result CreateD(const Path& directoryPath) final;
		virtual Result DeleteD(const Path& directoryPath) final;

		virtual b8 FileExists(const Path& filePath) final;
		virtual b8 DirectoryExists(const Path& directoryPath) final;
	};
}