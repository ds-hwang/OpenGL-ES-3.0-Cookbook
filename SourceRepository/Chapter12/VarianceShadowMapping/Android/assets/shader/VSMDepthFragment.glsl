#version 300 es
precision mediump float;

in vec4    position;
layout(location = 0) out vec4 FinalColor;

void main() {
    float depth = position.z / position.w ;
    //Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system
    depth = depth * 0.5 + 0.5;			
    
    float M1 = depth;           // Moment 1
    float M2 = depth * depth;   // Moment 2
    
    // Bias filtered region
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    moment2 += 0.25*(dx*dx+dy*dy) ;
    
    
    FinalColor = vec4( moment1,moment2, 0.0, 0.0 );
}


