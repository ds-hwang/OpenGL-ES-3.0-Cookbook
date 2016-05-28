#version 300 es

// Vertex information
layout(location = 0) in vec3  VertexPosition;
layout(location = 1) in vec2  VertexTexCoord;

out vec2 TexCoord;
uniform mat4 ModelViewProjectionMatrix;

void main( void ) {
    TexCoord = VertexTexCoord;
    vec4 glPos = ModelViewProjectionMatrix * vec4(VertexPosition,1.0);
    vec2 Pos = sign(glPos.xy);
    gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition,1.0);
}