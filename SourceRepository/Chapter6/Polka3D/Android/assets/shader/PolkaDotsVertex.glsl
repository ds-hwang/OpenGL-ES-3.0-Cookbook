#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat3    NormalMatrix;

out vec3    ObjectCoord;

void main() 
{
    ObjectCoord= VertexPosition.xyz;

    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}
