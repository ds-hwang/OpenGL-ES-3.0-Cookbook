#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D FontTexture;
uniform vec4 TextColor;
layout(location = 0) out vec4 outColor;

void main() {
    vec4 texcol = texture(FontTexture, TexCoord);
    outColor    = vec4(vec3(TextColor.r, TextColor.g, TextColor.b), texcol.a);
}