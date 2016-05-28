#version 300 es
precision highp float;

in vec4 LightIntensity;
layout(location = 0) out vec4 outColor;

void main( void ) {
    outColor = vec4(LightIntensity);
}
