#version 300 es
precision mediump float;


layout(location = 0) out vec4 outColor;

in vec3  ObjectCoord;
float Side      = 0.30;
float DotSize   = 0.13;
vec3 Cube       = vec3(Side, Side, Side);
vec3 RenderColor= vec3(0.0, 0.0, 0.0);

// Front face Model(mesh) color
vec3 ModelColor = vec3(1.0, 1.0, 1.0);
// Front face Model(mesh) color
vec3 DotColor   = vec3(0.4, 0.5, 1.0);

// Back face Model(mesh) color
vec3 BackSideModelColor = vec3(0.0, 1.0, 0.0);
// Back face polka dot color
vec3 BackSideDotColor   = vec3(1.0, 1.0, 1.0);


void main() {

    float insideCircle, length;
    vec3 position = mod(ObjectCoord, Cube) - Cube/2.0;
    length        = sqrt( position.x * position.x + position.y * position.y +position.z * position.z );
    //length        = length( position );
    insideCircle  = step(length,DotSize);
   
    // Determine final render output color based on front and back shading
    if (gl_FrontFacing){
        RenderColor  = vec3(mix(ModelColor, DotColor, insideCircle));
        outColor = vec4(RenderColor , 1.0);

    }
    else{
        RenderColor  = vec3(mix(BackSideModelColor, BackSideDotColor, insideCircle));
        outColor = vec4(RenderColor, 1.0);
    }
}

