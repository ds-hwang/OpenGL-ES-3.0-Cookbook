#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;
uniform int     Wobble;
out vec3    nNormal;
out vec3    eyeCoord;
out vec3    ObjectCoord;

void main() 
{
    nNormal   = normalize ( NormalMatrix * Normal );
    eyeCoord       = vec3 ( ModelViewMatrix * VertexPosition );
    ObjectCoord= VertexPosition.xyz;

    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}
