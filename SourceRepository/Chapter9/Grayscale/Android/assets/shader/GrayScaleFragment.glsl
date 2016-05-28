#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;
uniform float ScreenCoordX;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 rgb = texture(Tex1, TexCoord);
    if(gl_FragCoord.x > ScreenCoordX){
        const vec3 luminanceWeight = vec3(0.2125, 0.7154, 0.0721);
        float luminace = dot(rgb.rgb, luminanceWeight);
        outColor = vec4(luminace, luminace, luminace, rgb.a);
    }
    else{
        outColor = rgb;
    }
}