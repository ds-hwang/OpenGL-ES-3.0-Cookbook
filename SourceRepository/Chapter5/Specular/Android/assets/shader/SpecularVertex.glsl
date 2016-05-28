#version 300 es

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

// Specular Light and Material information
uniform vec3    MaterialSpecular;
uniform vec3    LightSpecular;
uniform vec3    LightPosition;
uniform float   ShininessFactor;

out vec4    FinalColor;

void main() 
{
    vec3 nNormal = normalize( NormalMatrix * Normal );
    vec3 eyeCoord= vec3( ModelViewMatrix * VertexPosition );
    vec3 nLight  = normalize( LightPosition - eyeCoord);
    vec3 V       = -normalize( eyeCoord );
    vec3 R       = reflect( -nLight, nNormal );
    
    float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    vec3 specular	= MaterialSpecular * LightSpecular;
    FinalColor   	= vec4( sIntensity * specular, 1.0 );

    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}