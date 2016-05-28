#version 300 es

// Vertex information
layout(location = 0) in vec3  position;
layout(location = 1) in vec2  texcoord;

uniform mat4 worldMatrix;
uniform mat4 viewProjectionMatrix;
uniform float lifeFactor;

out vec2 texCoord;
out float life;

void main( void ) {
    texCoord            = texcoord;
    life                = lifeFactor;
    gl_Position         = viewProjectionMatrix * vec4( position, 1.0 );
}
