#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;
uniform sampler2D PosTexture, NormalTexture, ColorTexture;
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;

// Light property
uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;
uniform float   ShininessFactor;

uniform vec3    LightPosition;
vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;

vec3 diffuseModel( vec3 pos, vec3 norm, vec3 diff )
{
    norm = normalize(norm);
    vec3 s = normalize(vec3(0.00001, 20.0, 30.0) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse =  diff * sDotN;
    
    return diffuse;
}
vec3 GouraudShading(vec3 eyeCoord, vec3 normalCoord, vec3 MaterialDiffuse)
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


layout(location = 0) out vec4 outColor;

void main() {
    // Retrieve position and normal information from textures
    vec3 pos = vec3( texture( PosTexture, TexCoord ) );
    vec3 norm = vec3( texture( NormalTexture, TexCoord ) );
    vec3 diffColor = vec3( texture(ColorTexture, TexCoord) );
    
    outColor = vec4( diffuseModel(pos,norm,diffColor), 1.0 );
    //outColor = vec4( GouraudShading(pos,norm,diffColor), 1.0 );
    
    ///////////////////////////
    //outColor = vec4(texture(Tex1, TexCoord).r);
//    outColor = vec4(texture(Tex1, TexCoord).rgb, 1.0);
//    outColor = vec4(texture(PosTexture, TexCoord).rgb, 1.0);
//    outColor = vec4(texture(NormalTexture, TexCoord).rgb, 1.0);
    //outColor = vec4(texture(ColorTexture, TexCoord).rgb, 1.0);
}