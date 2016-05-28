#version 300 es

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in mat4 MODELVIEWPROJECTIONMATRIX;

out vec4 Color;

void main() 
{
  gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
  Color = VertexColor;
}