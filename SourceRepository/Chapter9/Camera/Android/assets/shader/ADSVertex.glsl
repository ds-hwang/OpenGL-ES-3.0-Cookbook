// Vertex information
attribute vec4  VertexPosition;
attribute vec3  Normal;

// Model View Project matrix
uniform mat4    MODELVIEWPROJECTIONMATRIX;
uniform mat4    MODELVIEWMATRIX;
uniform mat3    NormalMatrix;

varying vec3    nNormal;
varying vec3    pos;

void main() 
{
    nNormal   = normalize ( NormalMatrix * Normal );
    pos       = vec3 ( MODELVIEWPROJECTIONMATRIX * VertexPosition );

    gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
}