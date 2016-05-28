#version 300 es

uniform mat4 ModelViewProjectMatrix;

layout(location = 0) in vec3  VertexPosition;
layout(location = 1) in vec2  VertexTexCoord;
layout(location = 2) in vec4  VertexColor;
out vec4 TriangleColor;
out vec2 TexCoord;

void main() {
    gl_Position   = ModelViewProjectMatrix*vec4(VertexPosition,1.0);
    TriangleColor = VertexColor;
    TexCoord = VertexTexCoord;
}