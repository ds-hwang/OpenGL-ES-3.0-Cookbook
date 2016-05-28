#version 300 es
precision mediump float;

// Material property
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;
uniform float  Time;

// Light property
uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;
uniform float   ShininessFactor;

uniform vec3    LightPosition;
uniform float   ScreenCoordX;

// Flag to enable and disable Adaptive anti-aliasing
uniform int     EnableAdaptiveAA;

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

in float objectY;
float Frequency = 6.0;

// Reference: OpenGL Shading Language by Randi Rost
void main() {
    if(gl_FragCoord.x < ScreenCoordX+1.0 && gl_FragCoord.x > ScreenCoordX-1.0){
        FinalColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    
    float offset    = Time/10.0;

    // GENERATE fractional value 0.0, 0.1, ........, 0.9
    float sawtooth  = fract((objectY+offset) * Frequency);
    
    // Produce values in the range between [-1, 1]
    float triangle  = 2.0 * sawtooth - 1.0;
    
    // Produce continous range from [ 1.0 ..... 0.0 ..... 1.0 ]
    triangle        = abs(triangle);
    // 0.0 = 0.0 - 1.0 = -1.0
    // 0.1 = 0.2 - 1.0 = -0.8
    // 0.2 = 0.4 - 1.0 = -0.6
    // 0.3 = 0.6 - 1.0 = -0.4
    // 0.4 = 0.8 - 1.0 = -0.2
    // Similarly,
    // 0.5 = 0.0
    // 0.6 = 0.2
    // 0.7 = 0.4
    // 0.8 = 0.6
    // 0.9 = 0.8
    // 1.0 = 1.0

    float dp        = length(vec2 (dFdx(objectY+offset), dFdy(objectY+offset)));
    float edge      = dp * Frequency * 4.0;
    float square    = 0.0;

    if (gl_FragCoord.x < ScreenCoordX){
        square      = step(0.5, triangle);
    }
    else{
        square      = smoothstep(0.5 - edge, 0.5 + edge, triangle);
    }
    
    FinalColor = vec4 (vec3 (square)*PhongShading(), 1.0);
}


