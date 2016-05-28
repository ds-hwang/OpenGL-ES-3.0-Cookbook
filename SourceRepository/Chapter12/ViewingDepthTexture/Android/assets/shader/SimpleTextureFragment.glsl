#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(vec3(texture(Tex1, TexCoord).r), 1.0);
    // Make the image appear darker to make it clearly visible.
    outColor = pow( outColor, vec4(10.0) );
}