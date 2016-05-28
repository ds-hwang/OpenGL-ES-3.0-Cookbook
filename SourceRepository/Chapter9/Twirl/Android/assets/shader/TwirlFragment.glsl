#version 300 es
precision mediump float;

in vec2 TexCoord;
uniform sampler2D Tex1;
uniform float imageWidth;
uniform float imageHeight;

uniform float ScreenCoordX;
uniform float twirlRadius;
uniform float angle;
uniform vec2 center;

float radiusFactor = 3.0;
layout(location = 0) out vec4 outColor;

vec4 Twirl(sampler2D tex, vec2 uv, float angle){
    // Get the current texture size of the image.
    vec2 texSize = vec2(imageWidth, imageHeight);
    
    // Change the texture coordinate with respect to the image dimensions
    vec2 tc = (uv * texSize) - center;
    
    // Calcualte the distance of the current transforamted texture coordiante from the center.
    float distance = sqrt(tc.x*tc.x + tc.y*tc.y);// Also length(tc), can be use instead;
    
    if (distance < twirlRadius+angle*radiusFactor){
        // Transform the texture coordinate if inside the twirl radius.
        float percent   = (twirlRadius - distance) / twirlRadius;
        float theta     = percent * percent * angle;
        float sinus     = sin(theta);
        float cosine    = cos(theta);
        tc = vec2(dot(tc, vec2(cosine, -sinus)), dot(tc, vec2(sinus, cosine)));
    }
    vec4 color = texture(tex, (tc + center) / texSize);
    return color;
}

void main() {
    vec4 rgb = texture(Tex1, TexCoord);
    if(gl_FragCoord.x > ScreenCoordX){
        outColor = Twirl(Tex1, TexCoord, angle);
    }else{
        outColor = rgb;
    }
}