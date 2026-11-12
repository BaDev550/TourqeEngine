#pragma once

#include <Engine/Render/Sprite/SpriteVertex.h>

namespace Tourqe::Engine { // CHANGE TS
	static constexpr u32 MaxSprites = 1000; // TODO(0x): find a efficent 256< data to store sprite data

	static const SpriteVertex SpriteVertices[6] = {
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }, // bottom-left
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }, // bottom-right
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // top-right
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }, // bottom-left
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // top-right
		{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // top-left
	};

	struct SpritePushConstant {
		u64 spriteDataBDA = ~0ull;
	};

	struct SpriteData {
		Runtime::Math::Mat4 world;
		u32 frameDataIndex;
		u32 firstVertex;
		u32 vertexBufferID;
		u32 diffuseTextureID;
		u32 normalTextureID;
		u32 samplerID;
		u32 pad[2];
	};
}