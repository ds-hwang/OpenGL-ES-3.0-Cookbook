#version 300 es
in vec4 VertexPosition;
in vec4 VertexColor;
out vec4 VarColor;
uniform mat4 MODELVIEWPROJECTIONMATRIX;


void main()
{
    gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
    VarColor = VertexColor;
}