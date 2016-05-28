#version 300 es
precision mediump float;
layout(location = 0) out vec4 outColor;

in vec3  ObjectCoord;
// Size of the logical square
uniform float Side;
uniform float DotSize;
vec3 Cube       = vec3(Side, Side, Side);
vec3 RenderColor= vec3(0.0, 0.0, 0.0);

// Front and Back face Model(mesh)/polka dot color color
uniform vec3 ModelColor, DotColor, BackSideModelColor, BackSideDotColor;

void main() {

    float insideSphere, length;
    vec3 position = mod(ObjectCoord, Cube) - Cube/2.0;
    // Also, length() can be used
    length        = sqrt( (position.x*position.x) + (position.y*position.y) + (position.z*position.z) );
    insideSphere  = step(length,DotSize);
   
    // Determine final render output color based on front and back shading
    if (gl_FrontFacing){
        RenderColor  = vec3(mix(ModelColor, DotColor, insideSphere));
        outColor = vec4(RenderColor , 1.0);
    }
    else{
        RenderColor  = vec3(mix(BackSideModelColor, BackSideDotColor, insideSphere));
        outColor = vec4(RenderColor, 1.0);
    }
}

