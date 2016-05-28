#version 300 es
in vec4 VertexPosition;
in vec4 VertexColor;
out vec4 VarColor;


void main()
{
    gl_Position = VertexPosition;
    VarColor    = VertexColor;
}