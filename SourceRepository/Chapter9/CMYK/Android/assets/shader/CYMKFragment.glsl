#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;
uniform float ScreenCoordX;
uniform int caseCYMK;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 rgb = texture(Tex1, TexCoord);
    vec3 cmy = vec3(1.0)-rgb.rbg;
    float k = min(cmy.r, min(cmy.g, cmy.b));
    vec3 target = cmy - 0.1 * k;
    if(k<0.3){
        k = 0.0;
    }
    else{
        k = 0.9 * (k-0.3)/0.7;
    }
    
    vec4 cmyk = vec4(target, k);
    if(caseCYMK == 0){ // CYAN conversion
        outColor = vec4(vec3(1.0 - cmyk.x),rgb.a);
    }
    else if(caseCYMK == 1){  // MAGENTA conversion
        outColor = vec4(vec3(1.0 - cmyk.y),rgb.a);
    }
    else if(caseCYMK == 2){ // YELLOW conversion
        outColor = vec4(vec3(1.0 - cmyk.z),rgb.a);
    }
    else if(caseCYMK == 3){ // BLACK conversion
        outColor = vec4(vec3(1.0 - cmyk.w),rgb.a);
    }
    else{ // RGB
        outColor = rgb;
    }
}