#version 300 es
precision mediump float;
in vec4 TriangleColor;
out vec4 FragColor;
uniform float ScreenWidth;
uniform float ScreenHeight;
uniform float ScreenCoordX;
uniform float ScreenCoordY;

in vec2 TexCoord;

vec3 lightDir = normalize(vec3(0.5, 0.5, 1.0));

void main() {
    // Calculate the resolution
    vec2 resolution = vec2(ScreenWidth, ScreenHeight);

    // Calculate the center
    vec2 center     = vec2(resolution.x/2.0, resolution.y/2.0);

    lightDir = normalize(vec3((ScreenCoordX - center.x)/(ScreenWidth*0.5), (ScreenCoordY - center.y)/(ScreenHeight*0.5), 1.0));

    // Calculate the radius
    float radius    = 0.5;

    // Calculate the current fragment position from the center
    vec2 position   = TexCoord.xy - vec2(0.5, 0.5);
    
    // Calculate the height of each position, hence generate the Z coordinate
    float z         = sqrt(radius*radius - position.x*position.x - position.y*position.y);
    
    // Normalize the coordinate
    vec3 normal     = normalize(vec3(position.x, position.y, abs(z)));
    
    // Check the current position if falling under the locus.
    if (length(position) > radius) { // Outside
        FragColor = vec4(vec3(0.0,0.0,0.0), 0.0);
    } else { // Inside
        float diffuse = max(0.0, dot(normal, lightDir));
        FragColor = vec4(vec3(diffuse), 1.0);
    }
}
