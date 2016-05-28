#version 300 es

precision mediump float;
uniform mediump vec3    MaterialDiffuse;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(MaterialDiffuse, 0.70);
}
