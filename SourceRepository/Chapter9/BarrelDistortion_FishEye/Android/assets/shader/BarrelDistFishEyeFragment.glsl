#version 300 es
precision mediump float;
in vec2 TexCoord;
uniform sampler2D Tex1;
layout(location = 0) out vec4 outColor;

uniform float BarrelPower;
uniform float ScreenCoordX;

vec2 BarrelDistortion(vec2 p)
{
    float theta  = atan(p.y, p.x);
    float radius = sqrt(p.x*p.x + p.y*p.y);  // length() function can also be used.
    radius = pow(radius, BarrelPower);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return (p + 0.5);
}

vec2 xy, uv;
float distance;
void main()
{
    if(gl_FragCoord.x > ScreenCoordX){
        // The range of text coordinate is from (0,0) to (1,1)
        // Assuming center of the Texture coordinate system middle of the screen.
        // Shift all coordiante wrt to the new center.
        // This will be the new position vector of the displaced coordinate.
        xy = TexCoord - vec2(0.5);
        
        // Calculate the distance from the center point.
        distance = sqrt(xy.x*xy.x+xy.y*xy.y); // length() function can also be used.
        
        float radius = 0.35;
        // Apply the Barrel Distortion if the distance is with in the radius.
        // Our radius is half of the ST dimension.
        uv = (distance < radius ? BarrelDistortion(xy): TexCoord);        
        
        if( distance > radius-0.01 && distance < radius+0.01 ){
            outColor = vec4(1.0, 0.0, 0.0,1.0);
        }
        else{
            // Fetch the UV from Texture Sample
            outColor = texture(Tex1, uv);
        }
    }
    else{
        outColor = texture(Tex1, TexCoord);
    }
    
    return;
}

