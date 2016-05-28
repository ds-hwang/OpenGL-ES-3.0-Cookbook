#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D TexFragile;
uniform sampler2D TexWood;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 TextureFragile = texture(TexFragile, TexCoord);
    vec4 TextureWood    = texture(TexWood, TexCoord);
    outColor = mix(TextureWood, TextureFragile, TextureFragile.a);
}