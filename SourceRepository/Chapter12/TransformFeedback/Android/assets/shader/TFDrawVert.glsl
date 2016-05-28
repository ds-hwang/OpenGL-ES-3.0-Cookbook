#version 300 es                                              
#define ATTRIBUTE_POSITION      0                             
#define ATTRIBUTE_VELOCITY      1                             
#define ATTRIBUTE_SIZE          2                             
#define ATTRIBUTE_CURTIME       3                             
#define ATTRIBUTE_LIFETIME      4                             
                                                              
layout(location = ATTRIBUTE_POSITION) in vec2   inPosition;
layout(location = ATTRIBUTE_VELOCITY) in vec2   inVelocity;
layout(location = ATTRIBUTE_SIZE) in float      inSize;
layout(location = ATTRIBUTE_CURTIME) in float   inCurrentTime;
layout(location = ATTRIBUTE_LIFETIME) in float  inLifeTime;
                                                              
uniform float   time;
uniform vec2    acceleration;
uniform mat4    ModelViewProjectMatrix;

void main()                                                   
{                                                             
  float deltaTime = (time - inCurrentTime)/10.0;
  if ( deltaTime <= inLifeTime )
  {                                                           
     vec2 velocity  = inVelocity + deltaTime * acceleration;
     vec2 position  = inPosition + deltaTime * velocity;
     gl_Position    = ModelViewProjectMatrix * vec4(position, 0.0, 1.0 );
     gl_PointSize   = inSize * ( 1.0 - deltaTime / inLifeTime );
  }                                                           
  else                                                        
  {                                                           
     gl_Position    = vec4( -1000, -1000, 0, 0 );
     gl_PointSize   = 0.0;
  }
}