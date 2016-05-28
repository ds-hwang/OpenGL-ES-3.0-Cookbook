#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;
layout(location = 2) in vec2  TexCoords;


// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

out vec3    nNormal;
out vec3    eyeCoord;
out vec2    TextureCoord;

void main()
{
    nNormal      = normalize ( NormalMatrix * Normal );
    eyeCoord     = vec3 ( ModelViewMatrix * VertexPosition );
    TextureCoord = TexCoords;
    
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}