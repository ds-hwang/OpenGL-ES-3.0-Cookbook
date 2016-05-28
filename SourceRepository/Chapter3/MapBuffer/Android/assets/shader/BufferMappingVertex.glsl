#version 300 es

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec4 VertexColor;
uniform mat4 MODELVIEWPROJECTIONMATRIX;

out vec4 Color;

void main()
{
  gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
  gl_PointSize = 80.0;
  Color = VertexColor;
}