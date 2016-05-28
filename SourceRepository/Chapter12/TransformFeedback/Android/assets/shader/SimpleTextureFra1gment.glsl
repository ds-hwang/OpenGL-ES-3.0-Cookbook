#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;
layout(location = 0) out vec4 outColor;
uniform int Picked;

void main() {
    if(Picked == 1){
        
        outColor = vec4(0.0, 0.0, 1.0 ,texture(Tex1, TexCoord).a);
    }
    else{
        outColor = texture(Tex1, TexCoord);
    }
    //outColor = vec4(0.0, 0.0, 1.0 ,1.0);
    
}