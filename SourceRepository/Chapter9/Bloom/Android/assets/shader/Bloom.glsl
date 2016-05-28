#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 val = texture(Tex1, TexCoord);
    float brightness = 0.2126 * val.r + 0.7152 * val.g + 0.0722 * val.b;
    (brightness>0.6) ? outColor =vec4(1.0) : outColor =vec4(0.0);

}
