#version 300 es                                  
precision mediump float;                         
layout(location = 0) out vec4 fragColor;         
uniform vec4 color;
uniform sampler2D tex;

void main()                                      
{                                                
  vec4 texColor;                                 
  texColor      = texture( tex, gl_PointCoord );
  fragColor     = texColor * color;
  //fragColor     = vec4(1.0, 1.0, 0.0 ,1.0);
}
