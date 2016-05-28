#version 300 es
precision mediump float;
uniform samplerCube CubeMap;
in vec3    refractedDirection;

layout(location = 0) out vec4 outColor;
void main() {
    outColor = texture(CubeMap, refractedDirection);
    
}