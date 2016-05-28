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

in vec2     textureCoord;
in vec3     eyeCoord;
in mat3     tangentSpace;

layout(location = 0) out vec4 FinalColor;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;

vec3 GouraudShading( vec3 Normal, vec3 MaterialDiffuse ) {
    
    normalizeNormal   = normalize( Normal ) ;
    normalizeEyeCoord = normalize( eyeCoord);
    normalizeLightVec = normalize( (LightPosition - eyeCoord)  * tangentSpace);
    
    // Diffuse Intensity
    cosAngle = max( 0.0, dot(normalizeNormal, normalizeLightVec ));
    
    // Specular Intensity
    V = -normalizeEyeCoord; // Viewer's vector
    R = reflect( -normalizeLightVec, normalizeNormal); // Reflectivity
    sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );
    
    // ADS color as result of Material & Light interaction
    ambient    = MaterialAmbient  * LightAmbient;
    diffuse    = MaterialDiffuse  * LightDiffuse * cosAngle;
    specular   = MaterialSpecular * LightSpecular * sIntensity;
    
    return  ambient + diffuse + specular;
}


uniform sampler2D ImageTexture;
uniform sampler2D ImageTextureNormal;


void main() {
    // Lookup the normal from the normal map
    vec4 normalMap  = texture(ImageTextureNormal, vec2(1.0-textureCoord.x, textureCoord.y));
    
    // Change the normal coordinate from [0,1] to [-1 to 1]
    normalMap       = (2.0 * normalMap - 1.0);
    vec4 texColor   = texture(ImageTexture, vec2(1.0-textureCoord.x, textureCoord.y));
    FinalColor      = vec4( GouraudShading(normalMap.xyz, texColor.rgb), 1.0 );
    
    return;    
}

