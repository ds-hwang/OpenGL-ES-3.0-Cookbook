#version 300 es
precision mediump float;

uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;

uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;

vec3  BackFaceLightDiffuse = vec3(1.0, .50 ,0.30);

uniform vec3    LightPosition;
uniform float   ShininessFactor;
uniform int     toggleDiscardBehaviour;

in vec3    nNormal;
in vec3    eyeCoord;
layout(location = 0) out vec4 FinalColor;
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

in vec3  ObjectCoord;
uniform float Side;
uniform float DotSize;
vec3 Square     = vec3(Side, Side, Side);
vec3 RenderColor;

// Front and Back face Model(mesh)/polka dot color color
uniform vec3 ModelColor, DotColor, BackSideModelColor, BackSideDotColor;

void main() {

    float insideCircle, length;
    vec3 position = mod(ObjectCoord, Square) - Square/2.0;

    length = sqrt( (position.x*position.x) + (position.y*position.y) + (position.z*position.z) );
    insideCircle      = step(length,DotSize);
    
    // The toggleDiscardBehaviour change the behavior
    // automatically after fixed interval time.
    // The time is controlled from OpenGL ES program.
    if(toggleDiscardBehaviour == 0){
        if (insideCircle != 0.0)
            discard;
    }
    else{
        if (insideCircle == 0.0)
            discard;
    }
   
    // Determine final render output color based on front and back shading
    if (gl_FrontFacing){
        RenderColor  = vec3(mix(ModelColor, DotColor, insideCircle));
        FinalColor = vec4(RenderColor * PhongShading(true), 1.0);
    }
    else{
        RenderColor  = vec3(mix(BackSideModelColor, BackSideDotColor, insideCircle));
        FinalColor = vec4(RenderColor * PhongShading(false), 1.0);
    }
}

