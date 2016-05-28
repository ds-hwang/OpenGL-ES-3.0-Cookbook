#version 300 es
precision mediump float;

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;
layout(location = 2) in vec2  TexCoords;
layout (location = 3) in vec4 VertexTangent;


// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

uniform mediump vec3 LightPosition;

out vec2    textureCoord;
out vec3    eyeCoord;
out mat3    tangentSpace;


void main()
{
    // Transform normal and tangent to eye space
    vec3 norm     = normalize( NormalMatrix * Normal  );
    vec3 tang     = normalize( NormalMatrix * vec3(VertexTangent) );
    
    // Compute the binormal
    vec3 binormal =  cross( norm, tang );
    
    // Matrix for transformation to tangent space
     tangentSpace = mat3(
                              tang.x, binormal.x, norm.x,
                              tang.y, binormal.y, norm.y,
                              tang.z, binormal.z, norm.z );
    
    // Transform view direction to tangent space
    eyeCoord     = vec3 ( ModelViewMatrix * VertexPosition )*tangentSpace;
    
    textureCoord = TexCoords;
    gl_Position  = ModelViewProjectionMatrix * VertexPosition;
}