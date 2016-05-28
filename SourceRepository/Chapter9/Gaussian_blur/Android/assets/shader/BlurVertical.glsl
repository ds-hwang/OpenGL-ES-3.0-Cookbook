#version 300 es
precision mediump float;
in vec2 TexCoord;
uniform vec2 pixelSize;
uniform sampler2D Tex1;
layout(location = 0) out vec4 outColor;
uniform float ScreenCoordX;

uniform float PixOffset[5];
uniform float Weight[5]; // Gaussian weights
void main()
{
    // Draw the red line for showing the difference b/w orignal and Gaussian blur shader.
    if(gl_FragCoord.x < ScreenCoordX+1.0 && gl_FragCoord.x > ScreenCoordX-1.0){
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    
    if(gl_FragCoord.x > ScreenCoordX){
        // Unit pixel along T
        float dy = pixelSize.y;
        
        vec4 sum = texture(Tex1, TexCoord) * Weight[0];
        
        for( int i = 1; i < 5; i++ ) // Loop 4 times
        {
            sum += texture( Tex1, TexCoord + vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
            sum += texture( Tex1, TexCoord - vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
        }
        
        outColor = sum;
    }
    else{
        outColor = texture( Tex1, TexCoord);
    }

}

