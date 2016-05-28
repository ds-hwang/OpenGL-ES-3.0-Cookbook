#version 300 es
precision mediump float;

layout(location = 0) out vec4 FinalColor;
uniform sampler2D ImageGrassTexture;
in vec2    TextureCoord;

void main() {
    FinalColor = texture(ImageGrassTexture, TextureCoord);
}

