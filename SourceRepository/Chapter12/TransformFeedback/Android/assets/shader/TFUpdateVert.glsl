#version 300 es
#define NUM_PARTICLES           200
#define ATTRIBUTE_POSITION      0                                 
#define ATTRIBUTE_VELOCITY      1                                 
#define ATTRIBUTE_SIZE          2                                 
#define ATTRIBUTE_CURTIME       3                                 
#define ATTRIBUTE_LIFETIME      4                                 

uniform float               time;
uniform float               emissionRate;
uniform mediump sampler3D   noiseTex;
                                                                  
layout(location = ATTRIBUTE_POSITION) in vec2   inPosition;
layout(location = ATTRIBUTE_VELOCITY) in vec2   inVelocity;
layout(location = ATTRIBUTE_SIZE) in float      inSize;
layout(location = ATTRIBUTE_CURTIME) in float   inCurrentTime;
layout(location = ATTRIBUTE_LIFETIME) in float  inLifeTime;
                                                                  
out vec2    position;
out vec2    velocity;
out float   size;
out float   currentTime;
out float   lifeTime;
                                                                  
float randomValue( inout float seed )                             
{                                                                 
   float vertexId   = float( gl_VertexID ) / float( NUM_PARTICLES );
   vec3 texCoord    = vec3( time, vertexId, seed );
    seed             += (.10/float( NUM_PARTICLES ));
   return texture( noiseTex, texCoord ).r;
}                                                                 

void main()
{                                                                 
    float seed      = time;
    float lifetime  = (inCurrentTime - time)*10.0;
    if( lifetime <= 0.0 && randomValue(seed) < emissionRate )
    {
        position       = vec2( 0.0, -1.0 );
        velocity       = vec2( randomValue(seed) * 2.0 - 1.00,
                              randomValue(seed)  + 10.0 );
        size           = randomValue(seed) * 200.0;
        currentTime    = time;
        lifeTime       = 5.0;
    }
    else
    {
        position       = inPosition;
        velocity       = inVelocity;
        size           = inSize;
        currentTime    = inCurrentTime;
        lifeTime       = inLifeTime;
    }
    gl_Position = vec4( position, 0.0, 1.0 );
}