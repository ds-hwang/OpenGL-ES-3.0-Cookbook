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

layout(location = 0) out vec4 FinalColor;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;

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

    // ADS color as result of Material & Light interaction
    ambient    = MaterialAmbient  * LightAmbient;
    diffuse    = MaterialDiffuse  * LightDiffuse;
    specular   = MaterialSpecular * LightSpecular;

    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}


in vec3  ObjectCoord;
float Side      = 0.30;
float DotSize   = 0.13;
vec3 Cube       = vec3(Side, Side, Side);
vec3 RenderColor= vec3(0.0, 0.0, 0.0);

// Front face Model(mesh) color
vec3 ModelColor = vec3(1.0, 1.0, 1.0);
// Front face Model(mesh) color
vec3 DotColor   = vec3(0.4, 0.5, 1.0);

// Back face Model(mesh) color
vec3 BackSideModelColor = vec3(0.0, 1.0, 0.0);
// Back face polka dot color
vec3 BackSideDotColor   = vec3(1.0, 1.0, 1.0);


void main() {
    
    float insideCircle, length;
    vec3 position = mod(ObjectCoord, Cube) - Cube/2.0;
    length        = sqrt( position.x * position.x + position.y * position.y +position.z * position.z );
    //length        = length( position );
    insideCircle  = step(length,DotSize);
    
    // Determine final render output color based on front and back shading
    if (gl_FrontFacing){
        RenderColor  = vec3(mix(ModelColor, DotColor, insideCircle));
        FinalColor = vec4(RenderColor * PhongShading() , 1.0);
        
    }
    else{
        RenderColor  = vec3(mix(BackSideModelColor, BackSideDotColor, insideCircle));
        FinalColor = vec4(RenderColor * PhongShading(), 1.0);
    }
}


