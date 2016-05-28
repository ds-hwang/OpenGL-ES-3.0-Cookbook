#version 300 es
precision mediump float;

uniform sampler2D Tex1;
in vec2 texCoord;
in float life;

layout(location = 0) out vec4 outColor;

void main() {
    // directional light
    vec3 lightDir = normalize( vec3( 1.0, 1.0, 1.0 ) );
    // diffuse
    vec4 diffuseColor = vec4( 1, 1.0 - life, 0, 1 );
    vec4 texColor = texture( Tex1, texCoord );
    diffuseColor *= texColor;
    
    // final color
    vec4 color = vec4( 0.0, 0.0, 0.0, 1.0 );
    color.rgb = clamp( diffuseColor.rgb, 0.0, 1.0 );
    color.a = diffuseColor.a * life;
    
    // save it out
    outColor = vec4(texColor.xyz, 1.0);
    outColor = diffuseColor;
}
