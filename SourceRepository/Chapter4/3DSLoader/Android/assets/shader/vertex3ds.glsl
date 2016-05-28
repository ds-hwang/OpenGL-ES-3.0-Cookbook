#version 300 es
precision highp float;

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;

out vec4 LightIntensity;

uniform vec4 Ka;
uniform vec4 Kd;            
uniform vec4 Ks;
uniform float Ksh;
uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main( void ) {
    LightIntensity = Kd;
    gl_Position    = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);

}