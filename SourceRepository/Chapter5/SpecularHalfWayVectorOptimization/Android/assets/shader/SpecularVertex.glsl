#version 300 es

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ModelViewMatrix;

// Object's Light and Material information
uniform vec3 MaterialSpecular;
uniform vec3 LightSpecular;
uniform vec3 LightPosition;
uniform float ShininessFactor;

uniform mat3 NormalMatrix;
out vec4 FinalColor;

bool halfWayVectorOptimization = true;
void main() 
{
    vec3 nNormal = normalize( NormalMatrix * Normal );
    vec3 eyeCoord= vec3( ModelViewMatrix * VertexPosition );
    vec3 nLight  = normalize( LightPosition - eyeCoord );
    vec3 V       = normalize( -eyeCoord );
    
    vec3 H       = normalize (nLight + V);
    float sIntensity 	= pow( max( 0.0, dot( H, nNormal ) ), ShininessFactor );
    vec3 specular	= MaterialSpecular * LightSpecular;
    
    FinalColor   	= vec4( sIntensity * specular, 1 );

    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}