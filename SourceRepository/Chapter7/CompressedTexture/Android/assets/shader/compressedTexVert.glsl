#version 300 es

// Vertex information
layout(location = 0) in vec3  VertexPosition;
layout(location = 1) in vec2  VertexTexCoord;

out vec2 TexCoord;

uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main( void ) {
    TexCoord    = VertexTexCoord;
    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);
}