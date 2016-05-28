#version 300 es
precision mediump float;

in vec2             TexCoord;       // Texture coordiantes
uniform sampler2D   Tex1;           // FBO texture
uniform float       ScreenCoordX;   // X Screen Coordinate
uniform vec2        FBS;            // Frame Buffer Size

layout(location = 0) out vec4   outColor;

// http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
// Luminance Conversion
float FxaaLuma(vec3 rgb) {
    return rgb.y * (0.587/0.299) + rgb.x;
}

void main() {
    
    if(gl_FragCoord.x < ScreenCoordX+1.0 && gl_FragCoord.x > ScreenCoordX-1.0){
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    else if (gl_FragCoord.x < ScreenCoordX){
        outColor = texture(Tex1, TexCoord);
        return;
    }

    float FXAA_SPAN_MAX     = 8.0;
    float FXAA_REDUCE_MUL   = 1.0/8.0;
    float FXAA_REDUCE_MIN   = 1.0/128.0;
    
    vec3 rgbNW  = texture(Tex1,TexCoord+(vec2(-1.0,-1.0)/FBS)).xyz;
    vec3 rgbNE  = texture(Tex1,TexCoord+(vec2(1.0,-1.0)/FBS)).xyz;
    vec3 rgbSW  = texture(Tex1,TexCoord+(vec2(-1.0,1.0)/FBS)).xyz;
    vec3 rgbSE  = texture(Tex1,TexCoord+(vec2(1.0,1.0)/FBS)).xyz;
    vec3 rgbM   = texture(Tex1,TexCoord).xyz;
    
    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    float lumaM  = FxaaLuma(rgbM);
    
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
                          FXAA_REDUCE_MIN);
    
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
                                                        dir * rcpDirMin)) / FBS;
    
    vec3 rgbA = (1.0/2.0) * (texture(Tex1, TexCoord.xy + dir * (1.0/3.0 - 0.5)).xyz +
                             texture(Tex1, TexCoord.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (texture(Tex1, TexCoord.xy + dir * (0.0/3.0 - 0.5)).xyz + texture(Tex1, TexCoord.xy + dir * (3.0/3.0 - 0.5)).xyz);
    
    float lumaB = FxaaLuma(rgbB);
    
    if((lumaB < lumaMin) || (lumaB > lumaMax)){
        outColor=vec4(rgbA, 1.0);
    }else{
        outColor=vec4(rgbB, 1.0);
    }
}
