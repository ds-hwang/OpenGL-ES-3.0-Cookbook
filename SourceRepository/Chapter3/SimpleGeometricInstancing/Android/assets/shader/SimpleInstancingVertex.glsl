#version 300 es

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in float XOffsetPosition;

out vec4 Color;

uniform mat4 MODELVIEWPROJECTIONMATRIX;


void main() 
{
  gl_Position = MODELVIEWPROJECTIONMATRIX * (VertexPosition + vec4(XOffsetPosition, 0.0, 0.0, 0.0) );
  Color = VertexColor;
}