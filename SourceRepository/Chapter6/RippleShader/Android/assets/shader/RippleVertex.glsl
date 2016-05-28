#version 300 es
#define RIPPLE_AMPLITUDE 0.05
#define FREQUENCY 5.0
#define PI 3.14285714286

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

// Timer
uniform float   Time;

// Output variable for fragment shader
out vec3    nNormal;
out vec3    eyeCoord;

void main()
{
    nNormal   = normalize ( NormalMatrix * Normal );
    eyeCoord  = vec3 ( ModelViewMatrix * VertexPosition );
    vec4    VertexCoord = VertexPosition;
    float distance = length(VertexCoord);
    VertexCoord.y = sin( 2.0 * PI * distance * FREQUENCY + Time) * RIPPLE_AMPLITUDE;
    gl_Position = ModelViewProjectionMatrix * VertexCoord;
}
