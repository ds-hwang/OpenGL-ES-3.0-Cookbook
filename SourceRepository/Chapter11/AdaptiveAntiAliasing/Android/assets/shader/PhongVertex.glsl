#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

out vec3    normalCoord;
out vec3    eyeCoord;
out float   objectY;

void main()
{
    normalCoord = NormalMatrix * Normal;
    eyeCoord    = vec3 ( ModelViewMatrix * VertexPosition );
    objectY     = VertexPosition.y;
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}