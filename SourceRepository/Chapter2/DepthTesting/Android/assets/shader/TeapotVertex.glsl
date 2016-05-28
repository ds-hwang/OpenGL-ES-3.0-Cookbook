attribute vec4 VertexPosition;
uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main() 
{
  gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
}