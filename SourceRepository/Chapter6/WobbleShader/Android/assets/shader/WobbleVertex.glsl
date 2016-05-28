#version 300 es

//Define Constant for Wobble Shader
#define AMPLITUDE           1.2

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project Normal matrix
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
    vec4 VertexCoord = VertexPosition;
    VertexCoord.y += sin(VertexCoord.x + Time) * AMPLITUDE;
    gl_Position = ModelViewProjectionMatrix * VertexCoord;
}
