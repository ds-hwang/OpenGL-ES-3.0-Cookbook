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
in vec3    ObjectCoord;

layout(location = 0) out vec4 FinalColor;

// Brick uniform parameters
uniform vec3  BrickColor, MortarColor;
uniform vec2  RectangularSize, BrickPercent;

vec3 PhongShading()
{
    vec3 normalDir   = normalize ( nNormal );
    vec3 normalPosition = normalize( eyeCoord );
    vec3 nLight    = normalize( LightPosition - eyeCoord );
    
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

    return ambient + diffuse + specular;
}

vec3  color;
vec2  position, useBrick;

void main() {

    position = ObjectCoord.xy / RectangularSize;
    
    if (fract(position.y * 0.5) > 0.5){
        position.x += 0.5;
    }
    
    position    = fract(position);
    useBrick    = step(position, BrickPercent);
    color       = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    FinalColor  = vec4(color * PhongShading(), 1.0);
}

