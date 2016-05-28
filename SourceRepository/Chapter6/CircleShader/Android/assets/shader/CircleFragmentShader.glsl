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

vec3 PhongShading()
{
    vec3 normalDir      = normalize ( nNormal );
    vec3 normalPosition = normalize( eyeCoord );
    vec3 nLight         = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    float cosAngle = max( 0.0, dot( normalDir, nLight ));

    // Specular Intensity
    vec3 V       = -normalPosition;
    vec3 R       = reflect( -nLight, normalDir );
    float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    
    // ADS color as result of Material & Light interaction
    vec3 ambient    = MaterialAmbient  * LightAmbient;
    vec3 diffuse    = MaterialDiffuse  * LightDiffuse * cosAngle;
    vec3 specular   = MaterialSpecular * LightSpecular * sIntensity;
    
    return ambient + diffuse + sIntensity * specular;
}

// Model and Dot color
uniform vec3 ModelColor;
uniform vec3 DotColor;

// Output color for fragment
layout(location = 0) out vec4 FinalColor;

// Size of the logical square
uniform float Side;

// Dot size 25% of Square size
float DotSize   = Side * 0.25;
vec2 Square     = vec2(Side, Side);

void main() {
    vec2 position = mod(gl_FragCoord.xy, Square) - Square*0.5;
    float length = length(position);
    float inside = step(length,DotSize);
    
    FinalColor = vec4(mix(ModelColor, DotColor, inside), 1.0);
    FinalColor = FinalColor * vec4(PhongShading(), 1.0);
}

