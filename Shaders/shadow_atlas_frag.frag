#version 450
#include "core.glslh"

layout(location = 0) in vec2 vUV;
layout(location = 0) out float outDistance;

#include "light.glslh"

layout(push_constant) uniform ShadowMapPC {
    LightBuffer lightSbo;
    uint occluderTextureID;
    uint samplerID;
    uint frameDataBufferID;
    float resolution;
    uint lightCount;
    uint maxLights;
} pc;

const float THRESHOLD = 0.75;

void main() {
    uint lightIdx = uint(gl_FragCoord.y);
    if (lightIdx >= pc.lightCount) {
        outDistance = 1.0;
        return;
    }

    LightData light = pc.lightSbo.lights[lightIdx];
    mat4 viewProj = LoadMat4(pc.frameDataBufferID, 0u);

    float coord = gl_FragCoord.x / pc.resolution;
    float theta = coord * (2.0 * PI) - PI;
    vec2 dir = vec2(cos(theta), sin(theta));

    float minDist = 1.0;

    for (float step = 1.0; step < pc.resolution; step += 1.0) {
        float r = step / pc.resolution;
        vec2 worldPos = light.lightPos + dir * (r * light.lightRadius);

        vec4 clip = viewProj * vec4(worldPos, 0.0, 1.0);
        vec2 uv = clip.xy * 0.5 + 0.5;

        if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0) {
            vec4 occ = textureLod(GetBindlessTextureFromID(pc.occluderTextureID, pc.samplerID), uv, 0.0);
            if (occ.a > THRESHOLD && (occ.r + occ.g + occ.b) > 0.05) {
                minDist = r;
                break;
            }
        }
    }

    outDistance = minDist;
}