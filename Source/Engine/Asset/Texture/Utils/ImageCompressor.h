#pragma once

#include <Runtime/Definitions/Types.h>
#include <nvtt/nvtt.h>
#include <vector>

namespace Tourqe::Engine {
	struct NvttVectorOutputHandler : public nvtt::OutputHandler {
		std::vector<u8>& _buffer;

		NvttVectorOutputHandler(std::vector<u8>& buffer) : _buffer(buffer) {}

		virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel) override {
			_buffer.reserve(_buffer.size() + size);
		}

		virtual b8 writeData(const void* data, int size) override {
			const u8* src = static_cast<const u8*>(data);
			_buffer.insert(_buffer.end(), src, src + size);
			return true;
		}

		virtual void endImage() override {}
	};

	class ImageCompressor final {
	public:
		static b8 CompressImageToBC3(u8* rawData, u32 width, u32 height, std::vector<u8>& compressedImage);
		static b8 CompressImageToBC7(u8* rawData, u32 width, u32 height, std::vector<u8>& compressedImage);
	};
}