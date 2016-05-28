#version 300 es
precision mediump float;
uniform samplerCube CubeMapTexture;
in vec4 Vertex;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(CubeMapTexture, Vertex.xyz);
}