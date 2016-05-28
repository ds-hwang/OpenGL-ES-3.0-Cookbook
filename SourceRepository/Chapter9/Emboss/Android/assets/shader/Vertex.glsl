#version 300 es

// Vertex information
layout(location = 0) in vec3  VertexPosition;
layout(location = 1) in vec2  VertexTexCoord;

out vec2 TexCoord;
out vec2 TexCoordBlur;

uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main( void ) {
    TexCoord = VertexTexCoord;
    vec4 glPos = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);
    vec2 Pos = sign(glPos.xy);
    //Pos = sign(gl_Vertex.xy);
    TexCoordBlur = Pos * 0.5 - 0.5;
    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);
}