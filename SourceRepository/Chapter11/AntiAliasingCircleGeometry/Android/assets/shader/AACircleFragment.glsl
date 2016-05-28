#version 300 es
precision mediump float;

in vec2             TexCoord;       // Texture coordiantes
uniform float       ScreenCoordX;   // X Screen Coordinate
uniform vec3        PaintColor;     // circle color
uniform float       InnerRadius;    // inside radius
uniform float       OuterRadius;    // outside radius

float deltaAdjustment = 0.01;
layout(location = 0) out vec4   outColor;

void main() {
    if(gl_FragCoord.x < ScreenCoordX+1.0 && gl_FragCoord.x > ScreenCoordX-1.0){
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    
    float weight = 0.0f;
    float dx     = TexCoord.x - 0.5;
    float dy     = TexCoord.y - 0.5;
    float length = sqrt(dx * dx + dy * dy);
    
    // Calculate the weights
    if (gl_FragCoord.x < ScreenCoordX){
        weight = step( InnerRadius+deltaAdjustment, length );
    }
    else{
        weight = smoothstep( InnerRadius, OuterRadius, length );
    }

    outColor = mix( vec4(PaintColor, 1.0), vec4(PaintColor, 0.0), weight);
}
