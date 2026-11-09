#include <Runtime/PAL/Windows/WindowsContext.h>
#include <Runtime/PAL/Windows/WindowsWindow.h>
#include <Runtime/Memory/Allocator.h>
#include <Runtime/Definitions/Logger.h>
#include <Runtime/Definitions/Profiler.h>

#include <imgui_impl_win32.h>
#include <mio/mmap.hpp>
#include <Windows.h>

namespace Runtime::PAL {
	PlatformContext* CreatePlatformContext() {
		return Mem::Allocator::Construct<WindowsContext>();
	}

	Window* WindowsContext::CreateNewWindow(const WindowDesc& desc)
	{
		return Mem::Allocator::Construct<WindowsWindow>(desc);
	}

	void WindowsContext::ImGuiCreateContext(void* windowHandle) { ImGui_ImplWin32_Init(windowHandle); }
	void WindowsContext::ImGuiDestroyContext() { 
		ImGui_ImplWin32_Shutdown(); 
	}
	void WindowsContext::ImGuiNewFrame() { ImGui_ImplWin32_NewFrame(); }

	Result WindowsContext::WriteToFile(const Path& filePath, void* data, usize dataSize, usize dataOffset) {
		if (dataSize == 0) {
			Logger::Warn("WindowsIO", "Data size is 0, nothing to write");
			return Result("Data size is 0");
		}

		std::string pathDir = filePath.GetParentDirectory();
		if (!pathDir.empty() && !DirectoryExists(pathDir)) {
			Logger::Warn("WindowsIO", "Directory {} does not exist, creating directory", pathDir);
			Result result = CreateD(pathDir);
			if (!result) {
				Logger::Error("WindowsIO", "Failed to create directory {}", pathDir);
				return Result("Failed to create directory");
			}
		}

		if (!FileExists(filePath)) {
			Logger::Warn("WindowsIO", "File {} does not exist, creating file", filePath.GetPathStr());
		}

		const char* mode = (dataOffset > 0 && FileExists(filePath)) ? "r+b" : "wb";
		FILE* file = fopen(filePath.GetPathCStr(), mode);
		if (!file) {
			Logger::Error("WindowsIO", "Failed to open file for writing: {}", filePath.GetPathStr());
			return Result("Failed to open file");
		}

		if (dataOffset > 0) { fseek(file, static_cast<long>(dataOffset), SEEK_SET); }
		usize written = fwrite(data, 1, dataSize, file);
		fclose(file);

		if (written != dataSize) {
			Logger::Error("WindowsIO", "Failed to write all data to file: {}", filePath.GetPathStr());
			return Result("Incomplete write");
		}

		return Result();
	}

	Result WindowsContext::WriteToFile(const Path& filePath, const std::vector<u8>& data, usize dataSize, usize dataOffset) { return WriteToFile(filePath, const_cast<char*>(reinterpret_cast<const char*>(data.data())), dataSize, dataOffset); }

	std::vector<u8> WindowsContext::ReadFromFile(const Path& filePath) {
		if (!FileExists(filePath)) {
			Logger::Warn("WindowsIO", "File {} does not exist", filePath.GetPathStr());
			return {};
		}

		struct stat st;
		if (stat(filePath.GetPathCStr(), &st) == 0) {
			usize buffSize = static_cast<usize>(st.st_size);
			float buffSizeMB = ((float)buffSize / 1024 / 1024);

			if (!buffSize) {
				Logger::Warn("WindowsIO", "Empty file");
				return {};
			}

			Logger::Info("WindowsIO", "Reading buffer: {}", filePath.GetPathStr());
			Logger::Info("WindowsIO", "size: {:.2f}MB", buffSizeMB);

			std::vector<u8> fileBuffer((buffSize + sizeof(u8) - 1) / sizeof(u8));
			BEGIN_SCOPE(WindowsIOParseTime);
			FILE* file = fopen(filePath.GetPathCStr(), "rb");
			if (file) {
				fread(&fileBuffer[0], 1, buffSize, file);
				fclose(file);
			}
			else {
				Logger::Error("WindowsIO", "Failed to read file {}", filePath.GetPathStr());
				return {};
			}
			END_SCOPE(WindowsIOParseTime);
			return fileBuffer;
		}
		else {
			Logger::Warn("FileBuffer", "Failed to read buffer size");
			return {};
		}
		return {};
	}

	Result WindowsContext::MapFile(const Path& filePath, std::vector<u8>& mappedBuffer) {
		if (!FileExists(filePath)) {
			Logger::Warn("WindowsIO", "File {} does not exist", filePath.GetPathStr());
			return Result("File does not exist");
		}

		BEGIN_SCOPE(WindowsIOFileMap);
		Logger::Info("WindowsIO", "Mapping file: {}", filePath.GetPathStr());
		std::error_code err; // TODO(0x): write a custom memory mapper
		mio::mmap_source map(filePath.GetPathCStr());
		if (err) {
			Logger::Error("WindowsIO", "Failed to map file: {}", err.message());
			return Result("Failed to map file");
		}
		mappedBuffer.assign(map.begin(), map.end());
		END_SCOPE(WindowsIOFileMap);

		return Result();
	}

	Result WindowsContext::CreateF(const Path& filePath) {
		HANDLE file = CreateFile(
			filePath.GetPathCStr(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (file == INVALID_HANDLE_VALUE) {
			Logger::Error("WindowsIO", "Failed to create file: {}", filePath.GetPathStr());
			return Result("Failed to create file");
		}
		return Result();
	}

	Result WindowsContext::DeleteF(const Path& filePath) {
		BOOL result = DeleteFile(filePath.GetPathCStr());
		if (!result) {
			Logger::Error("WindowsIO", "Failed to delete file: {}", filePath.GetPathStr());
			return Result("Failed to delete file");
		}
		return Result();
	}

	Result WindowsContext::CreateD(const Path& directoryPath) {
		BOOL result = CreateDirectory(directoryPath.GetPathCStr(), NULL);
		if (!result) {
			Logger::Error("WindowsIO", "Failed to create directory: {}", directoryPath.GetPathStr());
			return Result("Failed to create directory");
		}
		return Result();
	}

	Result WindowsContext::DeleteD(const Path& directoryPath) {
		BOOL result = RemoveDirectory(directoryPath.GetPathCStr());
		if (!result) {
			Logger::Error("WindowsIO", "Failed to delete directory: {}", directoryPath.GetPathStr());
			return Result("Failed to delete directory");
		}
		return Result();
	}

	b8 WindowsContext::FileExists(const Path& filePath) {
		DWORD dwAttrib = GetFileAttributes(filePath.GetPathCStr());
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	b8 WindowsContext::DirectoryExists(const Path& directoryPath) {
		DWORD dwAttrib = GetFileAttributes(directoryPath.GetPathCStr());
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}
}