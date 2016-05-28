#version 300 es

precision mediump float;

// Material property
uniform vec3 MaterialAmbient;
uniform vec3 MaterialSpecular;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialDiffuseBackFace;

// Light property
uniform vec3 LightAmbient;
uniform vec3 LightSpecular;
uniform vec3 LightDiffuse;

uniform vec3    LightPosition;
uniform float   ShininessFactor;

in vec3    normalCoord;
in vec3    eyeCoord;

layout(location = 0) out vec4 FinalColor;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;
vec3 PhongShading( bool frontSide )
{
    normalizeNormal   = normalize ( normalCoord );
    normalizeLightVec = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    if ( frontSide){
        cosAngle = max( 0.0, dot( normalizeNormal, normalizeLightVec ));
    }
    else{
        cosAngle = max( 0.0, dot( -normalizeNormal, normalizeLightVec ));
    }
    
    // Specular Intensity
    V       = normalize( -eyeCoord );
    R       = reflect( -normalizeLightVec, normalizeNormal );
    sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );

    // ADS color as result of Material & Light interaction
    ambient    = MaterialAmbient  * LightAmbient;
    specular   = MaterialSpecular * LightSpecular;
    //vec3 diffuse;
    if ( frontSide){
        diffuse    = MaterialDiffuse  * LightDiffuse;
    }
    else{
        diffuse    =  MaterialDiffuseBackFace * LightDiffuse;
    }

    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}

void main() {
    vec4 fragColor = vec4( 0.0 );
    if (gl_FrontFacing){
        fragColor = vec4(PhongShading(true), 1.0);
    }
    else{
        fragColor = vec4(PhongShading(false), 1.0);
    }
    FinalColor = fragColor;
}

