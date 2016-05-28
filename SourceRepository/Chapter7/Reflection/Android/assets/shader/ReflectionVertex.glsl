#version 300 es

// Vertex information
layout(location = 0) in vec4  VertexPosition;
layout(location = 1) in vec3  Normal;
uniform vec3    CameraPosition;


// Model View Project matrix
uniform mat4    MODELVIEWPROJECTIONMATRIX;
uniform mat4    MODELMATRIX;
uniform mat3    NormalMatrix;

vec3    worldCoordPosition;
vec3    worldCoordNormal;
out vec3 reflectedDirection;

void main( void ) {
    worldCoordPosition = vec3( MODELMATRIX * VertexPosition );
    worldCoordNormal   = normalize(vec3( MODELMATRIX * vec4(Normal, 0.0)));
    if(worldCoordNormal.z < 0.0){
        worldCoordNormal.z = -worldCoordNormal.z;
    }
    vec3 incidenceRay    = normalize( CameraPosition - worldCoordPosition );
    reflectedDirection  = reflect(incidenceRay, worldCoordNormal );
    gl_Position         = MODELVIEWPROJECTIONMATRIX * VertexPosition;
}
