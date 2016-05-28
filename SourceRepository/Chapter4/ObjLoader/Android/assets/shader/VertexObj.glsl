#version 300 es

// Vertex information
layout(location = 0) in vec4  VertexPosition;

// Model View Project matrix
uniform mat4    MODELVIEWPROJECTIONMATRIX;
void main() 
{
    gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
    gl_PointSize = 5.0;
}