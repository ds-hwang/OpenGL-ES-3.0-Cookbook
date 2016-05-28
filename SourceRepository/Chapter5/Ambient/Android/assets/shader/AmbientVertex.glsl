#version 300 es

// Vertex information
layout(location = 0) in vec4  VertexPosition;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;

// Object's Light and Material information
uniform vec3    MaterialAmbient;
uniform vec3    LightAmbient;

out vec4 FinalColor;

void main() 
{
    vec3 ambient = MaterialAmbient  * LightAmbient;
    FinalColor   = vec4(ambient, 1);
    gl_Position  = ModelViewProjectionMatrix * VertexPosition;
}