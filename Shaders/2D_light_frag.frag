#version 450
#include "core.glslh"

layout(location = 0) in vec2 vUV;
layout(location = 1) in flat uint vLightIndex;
layout(location = 2) in vec4 vLightColor;
layout(location = 3) in float vLightIntensity;

layout(location = 0) out vec4 outColor;

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

float sampleShadow(vec2 tc, float r) {
    tc.x = fract(tc.x);
    float storedDist = textureLod(GetBindlessTextureFromID(pc.shadowMapID, pc.samplerID), tc, 0.0).r;
    return step(r, storedDist);
}

void main() {
    vec2 norm = vUV * 2.0 - 1.0;
    float r = length(norm);

    float theta = atan(norm.y, norm.x);
    float coord = (theta + PI) / (2.0 * PI);
    float rowV = (float(vLightIndex) + 0.5) / pc.maxLights;
    vec2 tc = vec2(coord, rowV);

    float blur = (1.0 / pc.resolution) * smoothstep(0.0, 1.0, r);

    float sum = 0.0;
    sum += sampleShadow(vec2(tc.x - 4.0 * blur, tc.y), r) * 0.05;
    sum += sampleShadow(vec2(tc.x - 3.0 * blur, tc.y), r) * 0.09;
    sum += sampleShadow(vec2(tc.x - 2.0 * blur, tc.y), r) * 0.12;
    sum += sampleShadow(vec2(tc.x - 1.0 * blur, tc.y), r) * 0.15;

    sum += sampleShadow(tc, r) * 0.16;

    sum += sampleShadow(vec2(tc.x + 1.0 * blur, tc.y), r) * 0.15;
    sum += sampleShadow(vec2(tc.x + 2.0 * blur, tc.y), r) * 0.12;
    sum += sampleShadow(vec2(tc.x + 3.0 * blur, tc.y), r) * 0.09;
    sum += sampleShadow(vec2(tc.x + 4.0 * blur, tc.y), r) * 0.05;

    float attenuation = smoothstep(1.0, 0.0, r);
    float intensity = sum * (attenuation * vLightIntensity);

    outColor = vec4(vLightColor.rgb * intensity, vLightColor.a * intensity);
}