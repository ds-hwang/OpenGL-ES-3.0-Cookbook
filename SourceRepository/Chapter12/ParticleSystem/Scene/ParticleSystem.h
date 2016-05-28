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

// define some MAX values
static const unsigned int MAX_PARTICLES = 128;
static const float MAX_LIFE = 20.0f;

// mesh vertex
typedef struct _Vertex
{
    glm::vec3 pos;
    glm::vec2 texCoord;
} Vertex;

// mesh
typedef struct _MeshParticle
{
    Vertex* vertices;
    unsigned int vertexCount;
} MeshParticle;


// define the particle properties
typedef struct _Particle
{
    // current particle position
    glm::vec3 pos;
    // current particle velocity
    glm::vec3 vel;
    // remaining particle life
    // used to determine opacity
    float life;
    // transform matrix sent to the shader
    glm::mat4 transform;
} Particle;


class ParticleSystem : public Model, public GestureEvent
{
    
public:
    // Constructor for Button
    ParticleSystem( Scene* parent, Model* model, ModelType type, glm::vec3* vertices, glm::vec2* textureCoordinates, char* texture);

    // Release the allocated memory assigned to a particle system
    virtual ~ParticleSystem();

    // Initialize our Model class
    void InitModel();
    
    // Allocate the memory to particles
    void InitParticles();

    // Load and compile draw shader
    void DrawShader();

    // Render the Model class
    void Update();
    
    // Render the particle system.
    void Render();

    // Render particles
    void RenderParticles();
    
    // particle simulation step
    // this is where particle behavior is defined
    void EmitParticles(float fElapsedTime );
    
    // Create
    MeshParticle* CreateQuadrilateral( void );

    // Touch Events
	void TouchEventDown( float x, float y );
    
	void TouchEventMove( float x, float y );
	
    void TouchEventRelease( float x, float y );

    bool IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint);
    
private:
    char TEX;
    Image* image;

    // List of particles
    Particle* particles;
    
    // Particle location
    glm::vec3 sourcePosition;
    
    // Particle transform matrix
    char worldUniform;

    // View Projection matrix
    char viewProjectionUniform;
    
    // Particle life span
    char life;
    
    // vertices used to draw the particle
    MeshParticle* particleMesh;
};
#endif // __ParticleSystem__
 
