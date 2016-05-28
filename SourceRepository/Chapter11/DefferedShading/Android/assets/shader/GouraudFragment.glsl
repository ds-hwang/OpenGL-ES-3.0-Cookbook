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
layout (location = 1) out vec4 PositionData;
layout (location = 2) out vec4 NormalData;
layout (location = 3) out vec4 ColorData;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;
uniform int Picked;
uniform int isLightPrespectivePass;



//vec3 GouraudShading(vec3 eyeCoord, vec3 normalCoord, vec3 MaterialDiffuse)
//{
//    normalizeNormal   = normalize( normalCoord );
//    normalizeEyeCoord = normalize( eyeCoord );
//    normalizeLightVec = normalize( LightPosition - eyeCoord );
//    
//    // Diffuse Intensity
//    cosAngle = max( 0.0, dot( normalizeNormal, normalizeLightVec ));
//    
//    // Specular Intensity
//    V = -normalizeEyeCoord; // Viewer's vector
//    R = reflect( -normalizeLightVec, normalizeNormal ); // Reflectivity
//    sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
//    
//    // ADS color as result of Material & Light interaction
//    ambient    = MaterialAmbient  * LightAmbient;
//    diffuse    = MaterialDiffuse  * LightDiffuse;
//    specular   = MaterialSpecular * LightSpecular;
//    
//    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
//}


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
//        PositionData = vec4(1.0, 0.0, 0.0, 1.0);
//        NormalData   = vec4(0.0, 1.0, 0.0, 1.0);
//        ColorData    = vec4(0.0, 0.0, 1.0, 1.0);
        PositionData    = vec4(eyeCoord, 1.0);
        NormalData      = vec4(normalize(normalCoord), 1.0);
        ambient    = MaterialAmbient  * LightAmbient;
        diffuse    = MaterialDiffuse  * LightDiffuse;
        specular   = MaterialSpecular * LightSpecular;
        ColorData       = vec4(ambient+diffuse+specular, 1.0);
        return;
    }

    if(Picked == 1){
        FinalColor = vec4(1.0, 0.0, 0.0 , 1.0);
    }
    else{
        vec3 diffAndSpec = GouraudShading();
        
        //float shadow = textureProj(ShadowMap, shadowCoord);
        // APPLY the Percentage Closer Edges and use sum of the
        // contributions from 4 texels around it
        float sum = 0.0;
        sum += textureProjOffset(ShadowMap, shadowCoord, ivec2(-1,-1));
        sum += textureProjOffset(ShadowMap, shadowCoord, ivec2(-1,1));
        sum += textureProjOffset(ShadowMap, shadowCoord, ivec2(1,1));
        sum += textureProjOffset(ShadowMap, shadowCoord, ivec2(1,-1));
        
        float shadow = sum * 0.25; // Average divide by 4
        // If the fragment is in shadow, use ambient light only.
        FinalColor = vec4(diffAndSpec * shadow + ambient, 1.0);
        
        // Correct the Gamma configuration
        FinalColor = pow( FinalColor, vec4(1.0 / 2.2) );
    }
    return;
}


