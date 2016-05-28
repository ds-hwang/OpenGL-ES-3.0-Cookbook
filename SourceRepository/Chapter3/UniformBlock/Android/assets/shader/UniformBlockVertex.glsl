#version 300 es

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec4 VertexColor;

out vec4 Color;

// Uniform Block Declaration
uniform Transformation{
    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
}Obj1;

void main()
{
    gl_Position = Obj1.ProjectionMatrix * Obj1.ViewMatrix * Obj1.ModelMatrix * VertexPosition;
    Color = VertexColor;
}