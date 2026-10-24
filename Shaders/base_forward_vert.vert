#version 450

#include "core.glslh"
#include "vertex_pulling.glslh"

layout(location = 0) out vec3 vNormal;
layout(location = 1) out vec2 vUV;
layout(location = 2) out flat uint vTextureID;
layout(location = 3) out flat uint vSamplerID;

layout(push_constant) uniform PcData {
	mat4 world;
	uint frameDataIndex;
	uint firstVertex;

	uint vertexBufferID;
	uint textureID;
	uint samplerID;
} uPc;

void main() {
	Vertex v = LoadVertex(uPc.vertexBufferID, uPc.firstVertex + uint(gl_VertexIndex));
	mat4 viewProj = LoadMat4(uPc.frameDataIndex, 0u);

	vTextureID = uPc.textureID;
	vSamplerID = uPc.samplerID;
	vNormal = mat3(uPc.world) * v.normal;
	vUV = v.uv;
	gl_Position = viewProj * uPc.world * vec4(v.position, 1.0f);
}