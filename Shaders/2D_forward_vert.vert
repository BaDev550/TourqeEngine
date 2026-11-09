#version 450

#include "core.glslh"
#include "vertex_pulling.glslh"
#include "2D_sprite.glslh"

layout(location = 0) out vec3 vNormal;
layout(location = 1) out vec2 vUV;
layout(location = 2) out flat uint vTextureID;
layout(location = 3) out flat uint vSamplerID;

layout(push_constant) uniform PcData {
	SpriteBuffer spriteSbo;
} uPc;

void main() {
	SpriteData s = uPc.spriteSbo.sprites[gl_InstanceIndex];
	Vertex v = LoadVertex(s.vertexBufferID, s.firstVertex + uint(gl_VertexIndex));
	mat4 viewProj = LoadMat4(s.frameDataIndex, 0u);
	mat4 world = s.world;

	vTextureID = s.diffuseTextureID;
	vSamplerID = s.samplerID;
	vNormal = mat3(world) * v.normal;
	vUV = v.uv;
	gl_Position = viewProj * world * vec4(v.position, 1.0f);
}