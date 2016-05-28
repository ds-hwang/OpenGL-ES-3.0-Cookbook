#version 300 es
precision mediump float;

uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;

uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;

uniform vec3    LightPosition;
uniform float   ShininessFactor;

in vec3    nNormal;
in vec3    eyeCoord;
layout(location = 0) out vec4 outColor;

vec3 PhongShading()
{
    vec3 normalDir   = normalize ( nNormal );
    vec3 normalPosition = normalize( eyeCoord );
    vec3 nLight    = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    float cosAngle = max( 0.0, dot( normalDir, nLight ));
    //sin(pos.y);
    // Specular Intensity
    vec3 V       = -normalPosition;
    vec3 R       = reflect( -nLight, normalDir );
    float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );

    // ADS color as result of Material & Light interaction
    vec3 ambient    = MaterialAmbient  * LightAmbient;
    vec3 diffuse    = MaterialDiffuse  * LightDiffuse;
    vec3 specular   = MaterialSpecular * LightSpecular;

    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}

void main() {
    outColor = vec4(PhongShading(), 1.0);
}

