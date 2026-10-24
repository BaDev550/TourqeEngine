#version 450
#include "core.glslh"

layout(location = 0) out vec4 vFragColor;

layout(location = 0) in vec3 vNormal;
layout(location = 1) in vec2 vUV;
layout(location = 2) in flat uint vTextureID;
layout(location = 3) in flat uint vSamplerID;

void main() {
	vec4 texColor = texture(GetBindlessTextureFromID(vTextureID, vSamplerID), vUV);
	vFragColor = texColor;
}