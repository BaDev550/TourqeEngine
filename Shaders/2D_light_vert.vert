#version 450
#include "core.glslh"

layout(location = 0) out vec2 vUV;
layout(location = 1) out flat uint vLightIndex;
layout(location = 2) out vec4 vLightColor;
layout(location = 3) out float vLightIntensity;

#include "light.glslh"

layout(push_constant) uniform LightPC {
    LightBuffer lightSbo;
    uint frameDataBufferID;
    uint shadowMapID;
    uint samplerID;
    float resolution;
    float maxLights;
    float _pad;
} pc;

void main() {
    const vec2 positions[6] = vec2[](
        vec2(-1.0, -1.0), vec2( 1.0, -1.0), vec2( 1.0,  1.0),
        vec2(-1.0, -1.0), vec2( 1.0,  1.0), vec2(-1.0,  1.0)
    );

    LightData light = pc.lightSbo.lights[gl_InstanceIndex];
    mat4 viewProj = LoadMat4(pc.frameDataBufferID, 0u);

    vec2 localPos = positions[gl_VertexIndex];
    vUV = localPos * 0.5 + 0.5;
    vLightIndex = uint(gl_InstanceIndex);
    vLightColor = light.lightColor;
    vLightIntensity = light.lightIntensity;

    vec2 worldPos = light.lightPos + localPos * light.lightRadius;
    gl_Position = viewProj * vec4(worldPos, 0.0, 1.0);
}