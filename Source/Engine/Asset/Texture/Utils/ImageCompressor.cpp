#include "ImageCompressor.h"

#include <Runtime/Definitions/Logger.h>
#include <Runtime/Definitions/Profiler.h>

namespace Tourqe::Engine {
	b8 ImageCompressor::CompressImageToBC3(u8* rawData, u32 width, u32 height, std::vector<u8>& compressedImage) {
		compressedImage.clear();

		{
			BEGIN_PROFILE_SCOPE("BC3Compression");

			nvtt::Surface surface;
			if (!surface.setImage(nvtt::InputFormat_BGRA_8UB, width, height, 1, rawData)) {
				Logger::Error("BC3Compression", "NVTT failed to set image data");
				return false;
			}
			surface.swizzle(2, 1, 0, 3);

			nvtt::CompressionOptions compOptions;
			compOptions.setFormat(nvtt::Format_BC3);
			compOptions.setQuality(nvtt::Quality_Fastest);

			NvttVectorOutputHandler outputHandler(compressedImage);
			nvtt::OutputOptions outputOptions;
			outputOptions.setOutputHandler(&outputHandler);
			outputOptions.setOutputHeader(false);

			nvtt::Context context;
			context.enableCudaAcceleration(true);

			b8 result = context.compress(surface, 0, 0, compOptions, outputOptions);

			if (result) {
				Logger::Info("BC3Compression", "Image compressed");
				return true;
			}
		}

		Logger::Error("BC3Compression", "NVTT compression failed");
		return false;
	}

	b8 ImageCompressor::CompressImageToBC7(u8* rawData, u32 width, u32 height, std::vector<u8>& compressedImage) {
		compressedImage.clear();

		{
			BEGIN_PROFILE_SCOPE("BC7Compression");

			nvtt::Surface surface;
			if (!surface.setImage(nvtt::InputFormat_BGRA_8UB, width, height, 1, rawData)) {
				Logger::Error("BC7Compression", "NVTT failed to set image data");
				return false;
			}
			surface.swizzle(2, 1, 0, 3);

			nvtt::CompressionOptions compOptions;
			compOptions.setFormat(nvtt::Format_BC7);
			compOptions.setQuality(nvtt::Quality_Fastest);

			NvttVectorOutputHandler outputHandler(compressedImage);
			nvtt::OutputOptions outputOptions;
			outputOptions.setOutputHandler(&outputHandler);
			outputOptions.setOutputHeader(false);

			nvtt::Context context;
			context.enableCudaAcceleration(true);

			b8 result = context.compress(surface, 0, 0, compOptions, outputOptions);

			if (result) {
				Logger::Info("BC7Compression", "Image compressed");
				return true;
			}
		}

		Logger::Error("Texture2DNVTT", "NVTT compression failed");
		return false;
	}
}