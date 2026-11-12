#version 450

layout(location = 0) out vec2 vUV;

void main() {
    const vec2 uvs[6] = vec2[](
        vec2(0.0, 0.0), vec2(2.0, 0.0), vec2(0.0, 2.0),
        vec2(0.0, 0.0), vec2(0.0, 2.0), vec2(2.0, 0.0)
    );
    vUV = uvs[gl_VertexIndex];
    gl_Position = vec4(vUV * 2.0 - 1.0, 0.0, 1.0);
}