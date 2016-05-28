#version 300 es
precision mediump float;

in vec2     TextureCoord;

layout(location = 0) out vec4 outColor;


uniform sampler2D ImageTexture;


void main() {
    vec4 texColor = texture(ImageTexture, TextureCoord);
    if( texColor.x < 0.1 && texColor.y < 0.1 && texColor.z < 0.1 ){
        discard;
    }
    if( texColor.x > 0.9 && texColor.y > 0.9 && texColor.z > 0.9 ){
        discard;
    }
    outColor = texColor;
    return;    
}

