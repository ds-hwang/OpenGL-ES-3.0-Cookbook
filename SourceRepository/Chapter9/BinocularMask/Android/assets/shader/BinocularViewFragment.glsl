#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;

uniform vec2 center;
uniform float horizontalAspectRatio;
uniform float verticalAspectRatio;

uniform float LensInnerRadius;
uniform float LensOuterRadius;
uniform vec4 BorderColor;

layout(location = 0) out vec4 outColor;
void main() {
    outColor = texture(Tex1, TexCoord);
    float dx = TexCoord.x - center.x;
    float dy = TexCoord.y - center.y;
    dx       *= horizontalAspectRatio;
    dy       *= verticalAspectRatio;
    float distance = sqrt(dx * dx + dy * dy);
    outColor =
    mix( outColor, BorderColor,
        smoothstep( LensInnerRadius, LensOuterRadius, distance )
        );
    return;
}