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

uniform sampler2D ShadowMap;

layout(location = 0) out vec4 FinalColor;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;
uniform int isLightPrespectivePass;

vec3 PhongShading()
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

    // DS color as result of Material & Light interaction
    diffuse    = MaterialDiffuse  * LightDiffuse;
    specular   = MaterialSpecular * LightSpecular;

    return ( cosAngle * diffuse ) + ( sIntensity * specular );
}
vec4 homogenShadowCoords;

float chebyshevComputeQuantity( float distance)
{
    // Get the two moments M1 and M2 in moments.x and moment.y respectively
    vec2 moments = texture(ShadowMap,homogenShadowCoords.xy).rg;
    
    // Current fragment is ahead of the object surface, therefore must be ligthed
    if (distance <= moments.x)
        return 1.0 ;
    
    float E_x2 = moments.y;
    float Ex_2 = moments.x * moments.x;

    // Computer the variance
    float variance = E_x2 - (Ex_2);
    
    float t = distance - moments.x;
    float pMax = variance / (variance + t*t);
    
    return pMax;
}

void main() {
    vec3 diff_Spec = PhongShading();
    
    // Calculate the homogenous coordinates
    homogenShadowCoords = shadowCoord/shadowCoord.w;
    
    // Calculate the quantity
    float shadow = chebyshevComputeQuantity(homogenShadowCoords.z);

    ambient    = MaterialAmbient  * LightAmbient;

    // If the fragment is in shadow, use ambient light only.
    FinalColor = vec4(diff_Spec * shadow + ambient, 1.0);
    
    // Correct the Gamma configuration
    FinalColor = pow( FinalColor, vec4(1.0 / 2.2) );
    return;
}


