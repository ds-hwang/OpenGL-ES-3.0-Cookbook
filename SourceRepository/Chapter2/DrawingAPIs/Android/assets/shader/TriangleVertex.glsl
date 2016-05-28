attribute vec4 VertexPosition;
attribute vec4 VertexColor;
varying vec4 VarColor;
uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main() 
{
  gl_Position = MODELVIEWPROJECTIONMATRIX * VertexPosition;
  VarColor = VertexColor;
}

/*attribute vec3 VertexPosition;


uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main()
{
    
    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);
}*/