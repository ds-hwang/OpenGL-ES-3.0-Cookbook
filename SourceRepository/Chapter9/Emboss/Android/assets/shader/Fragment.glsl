#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(Tex1, TexCoord);
}
