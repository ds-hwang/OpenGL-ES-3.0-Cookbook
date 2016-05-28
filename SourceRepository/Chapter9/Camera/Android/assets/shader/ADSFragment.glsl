uniform mediump vec3    MaterialAmbient;
uniform mediump vec3    MaterialSpecular;
uniform mediump vec3    MaterialDiffuse;

uniform mediump vec3    LightAmbient;
uniform mediump vec3    LightSpecular;
uniform mediump vec3    LightDiffuse;

uniform mediump vec3    LightPosition;
uniform mediump float   ShininessFactor;



//uniform mat3    NormalMatrix;
//varying vec4    FinalColor;
varying mediump vec3    nNormal;
varying mediump vec3    pos;

mediump vec3 PhongShading()
{
    mediump vec3 normalDir   = normalize ( nNormal );
    mediump vec3 normalPosition = normalize( pos );
    mediump vec3 nLight    = normalize( LightPosition - normalPosition );
    
    // Diffuse Intensity
    mediump float cosAngle = max( 0.0, dot( normalDir, nLight ));

    // Specular Intensity
    mediump vec3 V       = normalPosition;
    mediump vec3 R       = reflect( -nLight, normalDir );
    mediump float sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );

    // ADS color as result of Material & Light interaction
    mediump vec3 ambient    = MaterialAmbient  * LightAmbient;
    mediump vec3 diffuse    = MaterialDiffuse  * LightDiffuse;
    mediump vec3 specular   = MaterialSpecular * LightSpecular;

    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}

void main() {
  gl_FragColor = vec4(PhongShading(), 1.0);
  //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

