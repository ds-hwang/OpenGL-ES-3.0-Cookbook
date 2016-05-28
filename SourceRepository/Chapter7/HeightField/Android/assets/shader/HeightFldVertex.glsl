#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;
layout(location = 2) in vec2  TexCoords;


// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;

out vec2    TextureCoord;
out vec3    vertexColor;
uniform sampler2D HeightMapTexture;
uniform float heightFactor;
void main()
{
    TextureCoord    = TexCoords;
    vec4 height     = texture(HeightMapTexture, TexCoords);
    height          /= heightFactor;
    vec4 pos        = VertexPosition;
    gl_Position     = ModelViewProjectionMatrix * vec4(pos.x, height.r, pos.z, 1.0);
}