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
in vec3    pos;
in vec2     TextureCoord;

layout(location = 0) out vec4 outColor;

vec3 PhongShading( bool frontSide )
{
    vec3 nLight    = normalize( LightPosition - pos );
    
    // Diffuse Intensity
    float cosAngle = 0.0;
    if ( frontSide){
        cosAngle = max( 0.0, dot( nNormal, nLight ));
    }
    else{
        cosAngle = max( 0.0, dot( -nNormal, nLight ));
    }
    // Specular Intensity
    vec3 V       = -normalize( pos );
    vec3 R       = reflect( -nLight, nNormal );
    float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    
    // ADS color as result of Material & Light interaction
    vec3 ambient    = MaterialAmbient  * LightAmbient;
    vec3 specular   = MaterialSpecular * LightSpecular;
    vec3 diffuse;
    if ( frontSide){
        diffuse    = MaterialDiffuse  * LightDiffuse;
    }
    else{
        diffuse    = MaterialDiffuse  * BackFaceLightDiffuse;
    }
    
    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}


uniform sampler2D ImageTexture;
uniform sampler2D ImageTextureNormal;
in vec3 lightVec;//lightDir; 	  // interpolated surface local coordinate light direction
in vec3 eyeVec;//viewDir;     // interpolated surface local coordinate view direction


void main() {
    vec3 texcolor = vec3(texture(ImageTexture, TextureCoord));
    float dist = length(pos - LightPosition);
    float att = 1.0/(1.0 + 0.01*dist + 0.001*dist*dist);
    vec3 amb = texcolor * LightAmbient;
    
    vec3 surf2light = normalize(lightVec);
    vec3 norm = normalize(texture(ImageTextureNormal, TextureCoord)).xyz*2.0 - 1.0;
    float dcont = max(0.0, dot(norm, surf2light));
    vec3 diff = dcont * texcolor * LightDiffuse;
    
    vec3 surf2view = normalize(eyeVec);
    vec3 reflct = reflect(-lightVec, norm);
    float scont 	= pow( max( 0.0, dot( surf2view, reflct ) ), ShininessFactor );
    vec3 speclgt = scont * LightSpecular;
    outColor = vec4((amb + diff + speclgt)*att, 1.0);
    //vec3 R       = reflect( -nLight, nNormal );
    //float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    //outColor = texture(ImageTextureNormal, TextureCoord) * vec4(PhongShading(true), 1.0);
    return;    
}

