#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;

out vec4    position;

void main()
{
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
    position = gl_Position;
}