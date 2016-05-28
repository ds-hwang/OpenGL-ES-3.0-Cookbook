//
//  ParticleSystem.h
//  Particle system example
//
//  Created by macbook on 1/3/15.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __ParticleSystem__
#define __ParticleSystem__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "RendererEx.h"

#include <string>
#include "PngImage.h"
#include "Event.h"

using namespace std;
using namespace namespaceimage;

#define NUM_PARTICLES   200
#define EMISSION_RATE   0.4f
#define ACCELERATION   -1.0f

#define ATTRIBUTE_POSITION      0
#define ATTRIBUTE_VELOCITY      1
#define ATTRIBUTE_SIZE          2
#define ATTRIBUTE_CURTIME       3
#define ATTRIBUTE_LIFETIME      4

typedef struct
{
    float position[2];
    float velocity[2];
    float size;
    float curtime;
    float lifetime;
} Particle;

typedef struct
{
    // Handle to a program object
    GLuint emitProgramObject;
    GLuint drawProgramObject;
    
    // Emit shader uniform locations
    GLint emitTimeLoc;
    GLint emitEmissionRateLoc;
    GLint emitNoiseSamplerLoc;
    
    // Draw shader uniform location
    GLint drawTimeLoc;
    GLint drawColorLoc;
    GLint drawAccelerationLoc;
    GLint samplerLoc;
    
    // Texture handles
    GLuint textureId;
    GLuint noiseTextureId;
    
    // Particle vertex data
    Particle particleData[ NUM_PARTICLES ];
    
    // Particle VBOs
    GLuint particleVBOs[2];
    
    // Index into particleVBOs (0 or 1) as to which is the source.
    // Ping-pong between the two VBOs
    GLuint curSrcIndex;
    
    // Current time
    float time;
    
    // synch object to synchronize the transform feedback results and the draw
    GLsync emitSync;
    
} UserData;

class ParticleSystem : public Model, public GestureEvent
{
    
public:
    // Constructor for Button
    ParticleSystem( Scene* parent, Model* model, ModelType type, glm::vec3* vertices, glm::vec2* textureCoordinates, char* texture);

    // Destructor for ParticleSystem
    virtual ~ParticleSystem();

    // Initialize our Model class
    void InitModel();

    // Allocate the memory to particles
    void InitParticles();
    
    unsigned int Create3DNoiseTexture ( int textureSize, float frequency );

    // Load and compile draw shader
    void DrawShader();

    void UpdateShader();

    // Render the Model class
    void Update();
    
    void Render();
    
    void RenderParticles();

    void EmitParticles(float deltaTime );
    
    void SetupVertexAttributes ( GLuint vboID );
    
    // Touch Events
	void TouchEventDown( float x, float y );
    
	void TouchEventMove( float x, float y );
	
    void TouchEventRelease( float x, float y );

    bool IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint);

private:
    ////////////////////////////////////////////////////////
    // Handle to a program object
    GLuint emitProgramObject;
    GLuint drawProgramObject;
    
    // Emit shader uniform locations
    GLint emitTimeLoc;
    GLint emitEmissionRateLoc;
    GLint emitNoiseSamplerLoc;
    
    // Draw shader uniform location
    GLint drawTimeLoc;
    GLint drawColorLoc;
    GLint drawAccelerationLoc;
    GLint samplerLoc;
    
    // Texture handles
    GLuint textureId;
    GLuint noiseTextureId;
    
    // Particle vertex data
    Particle particleData[ NUM_PARTICLES ];
    
    // Particle VBOs
    GLuint particleVBOs[2];
    
    // Index into particleVBOs (0 or 1) as to which is the source.
    // Ping-pong between the two VBOs
    GLuint curSrcIndex;
    
    // Current time
    float time;
    
    // synch object to synchronize the transform feedback results and the draw
    GLsync emitSync;
    ////////////////////////////////////////////////////////
    char MVP;
    char TEX;
    Image* image;
//    glm::vec2 texCoordinates[4];
//    glm::vec3 vertices[4];
    char* textureImage;
};
#endif // __ParticleSystem__
 
