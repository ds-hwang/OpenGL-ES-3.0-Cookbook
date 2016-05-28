attribute vec3 VertexPosition;


uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main()
{
    
    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);
}