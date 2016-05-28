#version 300 es

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec4 VertexColor;

out vec4 Color;

uniform mat4 MODELVIEWPROJECTIONMATRIX;

void calculatePosition(in mat4 MVP, in vec4 vp, out vec4 position){
     position = MVP * vp;
}

void main() 
{
    vec4 position;
    calculatePosition(MODELVIEWPROJECTIONMATRIX, VertexPosition, position);
    gl_Position = position;
    Color = VertexColor;
}