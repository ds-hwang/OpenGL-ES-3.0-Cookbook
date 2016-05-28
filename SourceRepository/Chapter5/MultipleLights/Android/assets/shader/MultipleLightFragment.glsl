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

uniform vec3    LightPositionArray[4];
uniform vec3    LightDiffuseArray[4];
uniform float   ShininessFactor;

in vec3    normalCoord;
in vec3    eyeCoord;

layout(location = 0) out vec4 FinalColor;


vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;

vec3 PhongShading( int index )
{
    normalizeNormal   = normalize( normalCoord );
    normalizeEyeCoord = normalize( eyeCoord );
    normalizeLightVec = normalize( LightPositionArray[index] - eyeCoord );
    
    // Diffuse Intensity
    cosAngle = max( 0.0, dot( normalizeNormal, normalizeLightVec ));
    
    // Specular Intensity
    V = -normalizeEyeCoord; // Viewer's vector
    R = reflect( -normalizeLightVec, normalizeNormal ); // Reflectivity
    sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    
    // ADS color as result of Material & Light interaction
    ambient    = MaterialAmbient  * LightAmbient;
    diffuse    = MaterialDiffuse  * LightDiffuseArray[index];
    specular   = MaterialSpecular * LightSpecular;
    
    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );

}

void main() {
   vec4 multipleLightColor = vec4( 0.0 );
   for (int i=0; i<4; i++){
       multipleLightColor += vec4(PhongShading(i), 1.0);
   }

    FinalColor = multipleLightColor;
}

