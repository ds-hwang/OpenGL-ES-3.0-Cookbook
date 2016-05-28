#version 300 es
precision mediump float;

// Material property
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;

// Light property
uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;
uniform float   ShininessFactor;

uniform vec3    LightPosition;

in vec3    normalCoord;
in vec3    eyeCoord;
in vec4    shadowCoord;

uniform lowp sampler2DShadow ShadowMap;

layout(location = 0) out vec4 FinalColor;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;
uniform int isLightPrespectivePass;

vec3 GouraudShading()
{
    normalizeNormal   = normalize( normalCoord );
    normalizeEyeCoord = normalize( eyeCoord );
    normalizeLightVec = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    cosAngle = max( 0.0, dot( normalizeNormal, normalizeLightVec ));

    // Specular Intensity
    V = -normalizeEyeCoord; // Viewer's vector
    R = reflect( -normalizeLightVec, normalizeNormal ); // Reflectivity
    sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );

    // ADS color as result of Material & Light interaction
    ambient    = MaterialAmbient  * LightAmbient;
    diffuse    = MaterialDiffuse  * LightDiffuse;
    specular   = MaterialSpecular * LightSpecular;

    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}

void main() {
    if(isLightPrespectivePass == 1){
        return;
    }

    vec3 diffAndSpec = GouraudShading();
    
    float shadow = textureProj(ShadowMap, shadowCoord);
    // If the fragment is in shadow, use ambient light only.
    FinalColor = vec4(diffAndSpec * shadow + ambient, 1.0);
    
    // Correct the Gamma configuration
    FinalColor = pow( FinalColor, vec4(1.0 / 2.2) );
    return;
}


