#version 300 es
precision mediump float;
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;

uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;
uniform vec3    BackFaceLightDiffuse;

uniform vec3    LightPosition;
uniform float   ShininessFactor;

in vec3    nNormal;
in vec3    eyeCoord;

vec3 PhongShading( bool frontSide )
{
    vec3 nLight    = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    float cosAngle = 0.0;
    if ( frontSide){
        cosAngle = max( 0.0, dot( nNormal, nLight ));
    }
    else{
        cosAngle = max( 0.0, dot( -nNormal, nLight ));
    }
    // Specular Intensity
    vec3 V       = -normalize( eyeCoord );
    vec3 R       = reflect( -nLight, nNormal );
    float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    
    // ADS color as result of Material & Light interaction
    vec3 ambient    = MaterialAmbient  * LightAmbient;
    vec3 specular   = MaterialSpecular * LightSpecular * sIntensity;
    vec3 diffuse;
    if ( frontSide){
        diffuse    = MaterialDiffuse  * LightDiffuse * cosAngle;
    }
    else{
        diffuse    = MaterialDiffuse  * BackFaceLightDiffuse * cosAngle;
    }
    
    return ambient + diffuse + specular;
}

in vec2    TextureCoord;
layout(location = 0) out vec4 FinalColor;

// Scale factor of the texture coord & Grid strip width
uniform float texCoordMultiplyFactor, stripWidth;

void main() {
    // multiplicationFactor scales the number of stripes
    vec2 t = TextureCoord * texCoordMultiplyFactor;
    
    // The stripWidth constant is used to define the width of the lines
    if (fract(t.s) < stripWidth  || fract(t.t) < stripWidth ){
        if (gl_FrontFacing){ //Front Face coloring
            FinalColor = vec4( PhongShading(true), 1.0);
        }
        else{ //Back Face coloring
            FinalColor = vec4( PhongShading(false), 1.0);
        }
    }else{
        discard;
    }
}

