#version 300 es

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project and Normal matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

// Object's Light and Material information
uniform vec3    MaterialDiffuse;
uniform vec3    LightDiffuse;
uniform vec3    LightPosition;

out vec4        FinalColor;

void main() 
{
    vec3 nNormal   = normalize ( NormalMatrix * Normal );
    vec3 eyeCoord  = vec3 ( ModelViewMatrix * VertexPosition );
    vec3 nLight    = normalize( LightPosition - eyeCoord );
    
    // Calculate cosine between Normal and Light vector
    float cosAngle = max( 0.0, dot( nNormal, nLight ));

    // Compute diffuse Light and Material interaction
    vec3 diffuse   = MaterialDiffuse  * LightDiffuse;

    // Calculate net intensity
    vec3 color 	   = cosAngle * diffuse;
    FinalColor 	   = vec4(color, 1);

    gl_Position    = ModelViewProjectionMatrix * VertexPosition;
}