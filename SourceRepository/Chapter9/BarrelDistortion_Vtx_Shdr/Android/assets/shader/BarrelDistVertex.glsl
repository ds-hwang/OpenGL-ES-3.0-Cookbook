#version 300 es
// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;

// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

out vec3    normalCoord;
out vec3    eyeCoord;

out vec3    ObjectCoord;
uniform float BarrelPower;

vec4 BarrelDistortion(vec4 p)
{
    vec2 v = p.xy / p.w;
    // Convert to polar coords:
    float radius = length(v);
    if (radius > 0.0)
    {
        float theta = atan(v.y,v.x);
        
        // Apply distortion:
        radius = pow(radius, BarrelPower);
        
        // Convert back to Cartesian:
        v.x = radius * cos(theta);
        v.y = radius * sin(theta);
        p.xy = v.xy * p.w;
    }
    return p;
}

void main()
{
    normalCoord = NormalMatrix * Normal;
    eyeCoord    = vec3 ( ModelViewMatrix * VertexPosition );
    ObjectCoord = VertexPosition.xyz;
    gl_Position = BarrelDistortion(ModelViewProjectionMatrix * VertexPosition);
}