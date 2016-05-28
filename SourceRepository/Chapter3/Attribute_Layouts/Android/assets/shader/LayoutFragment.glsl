#version 300 es
precision mediump float;

in vec4 Color;
float blendFactor = 0.8;
layout(location = 0) out vec4 outColor;

vec4 addBlend( in vec4 colorOpaque )
{
    return vec4(colorOpaque.x, colorOpaque.y, colorOpaque.z, blendFactor);
}

void main() {
    outColor = addBlend( Color );
}
