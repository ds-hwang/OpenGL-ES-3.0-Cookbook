attribute vec4 VertexPosition;

void main() 
{
  gl_Position = VertexPosition;
}

/*attribute vec3 VertexPosition;


uniform mat4 MODELVIEWPROJECTIONMATRIX;

void main()
{
    
    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition,1.0);
}*/