#version 300 es
precision mediump float;
in vec2 TexCoord;
uniform vec2 pixelSize;
uniform sampler2D Tex1;
uniform sampler2D RenderTex;
layout(location = 0) out vec4 outColor;
uniform float ScreenCoordX;

uniform float PixOffset[5];
uniform float Weight[5];
void main()
{
    // Draw the red line for showing the difference b/w orignal and bloomed-blur shader.
    if(gl_FragCoord.x < ScreenCoordX+1.0 && gl_FragCoord.x > ScreenCoordX-1.0){
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }

    // Apply blurred bloom
    if(gl_FragCoord.x > ScreenCoordX){
        float dy = pixelSize.y;
        
        vec4 scene = texture(RenderTex, TexCoord);
        vec4 sum = texture(Tex1, TexCoord) * Weight[0];
        for( int i = 1; i < 5; i++ )
        {
            sum += texture( Tex1, TexCoord + vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
            sum += texture( Tex1, TexCoord - vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
        }
        outColor = sum + scene;
    }
    else{
        outColor = texture(RenderTex, TexCoord);
    }
}

