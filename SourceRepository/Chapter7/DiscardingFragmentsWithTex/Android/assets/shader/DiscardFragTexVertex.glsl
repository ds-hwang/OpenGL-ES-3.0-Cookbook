#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;
layout(location = 2) in vec2  TexCoords;

out vec2     TextureCoord;
// Model View Project matrix
uniform mat4    MODELVIEWPROJECTIONMATRIX;

void main()
{
    TextureCoord = TexCoords;
    gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
}