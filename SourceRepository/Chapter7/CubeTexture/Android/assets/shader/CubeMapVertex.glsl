#version 300 es

layout(location = 0) in vec4  VertexPosition;
uniform mat4 MVP;
out vec4 Vertex;

void main( void ) {
    Vertex = VertexPosition;
    gl_Position = MVP * VertexPosition;
}
