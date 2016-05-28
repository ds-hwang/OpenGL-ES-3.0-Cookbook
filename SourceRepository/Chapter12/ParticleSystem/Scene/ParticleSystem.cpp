#include "ParticleSystem.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			    ( char * )"ParticleVertex.glsl"
#define FRAGMENT_SHADER_PRG		        ( char * )"ParticleFragment.glsl"
#else
#define VERTEX_SHADER_PRG			    ( char * )"shader/ParticleVertex.glsl"
#define FRAGMENT_SHADER_PRG		        ( char * )"shader/ParticleFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

// Namespace used
using std::ifstream;
using std::ostringstream;
using namespace glm;

#define GetUniform(X,Y) ProgramManagerObj->ProgramGetUniformLocation(X,Y);

MeshParticle* ParticleSystem::CreateQuadrilateral( void )
{
    // Quadrilateral
    //
    //  1-------0
    //  |     / |
    //  |   /   |
    //  | /     |
    //  2-------3
    //
    
    // Interleave quadrilateral vertices with position, normal and texture coordinates
    const Vertex quadVertices[] =
    {
        // Triangle 1:
        // Orientation [ 0, 1, 2 ]
        { {  1.0f,  1.0f,  0.0f },  { 1.0f, 1.0f } },
        { { -1.0f,  1.0f,  0.0f },  { 0.0f, 1.0f } },
        { { -1.0f, -1.0f,  0.0f },  { 0.0f, 0.0f } },
        
        // Triangle 2:
        // Orientation [ 0, 2, 3 ]
        { {  1.0f,  1.0f,  0.0f },  { 1.0f, 1.0f } },
        { { -1.0f, -1.0f,  0.0f },  { 0.0f, 0.0f } },
        { {  1.0f, -1.0f,  0.0f },  { 1.0f, 0.0f } },
    };
    const unsigned int quadVertexCount = 6;
    
    MeshParticle* quad    = ( MeshParticle* )malloc( sizeof( MeshParticle ) );
    quad->vertices        = ( Vertex* )malloc( sizeof( Vertex ) * quadVertexCount );
    memcpy( quad->vertices, quadVertices, quadVertexCount * sizeof( Vertex ) );
    quad->vertexCount     = quadVertexCount;
    
    return quad;
}
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
ParticleSystem::ParticleSystem( Scene* parent, Model* model, ModelType type, glm::vec3* vtx, glm::vec2* textureCoordinates, char* texture ):Model(parent, model, type)
{
	if (!parent)
		return;
    
	SceneHandler        = parent;
	modelType 			= ImageDemoType;
    
    char fname[MAX_PATH]= {""};
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/GLPIFramework/Images/" );
#endif
    
    strcat( fname, texture );
    image = new PngImage();
    image->loadImage(fname);
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
ParticleSystem::~ParticleSystem()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"square" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
    
    // free up all the memory that was allocated on init
    free( particleMesh );
    free( particles );
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ParticleSystem::InitModel()
{
    // Initialize the shader
    DrawShader();
    
    // Initialize the particles
    InitParticles();
    
    // Call base class
    Model::InitModel();
    return;
}

/*!
    Update function
 
	\param[in] None.
	\return None
 
 */
void ParticleSystem::Update ()
{
    static clock_t lastTime = clock();
    clock_t currentTime     = clock();
    float deltaTime         = (currentTime - lastTime)/(float)(CLOCKS_PER_SEC);
    lastTime                = currentTime;
    
    EmitParticles( deltaTime );
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ParticleSystem::Render()
{
    Update();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(program->ProgramID);
    
    // Disable culling
    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture (GL_TEXTURE0);
    //glUniform1i(TEX, 0);
    if (image) {
        glBindTexture(GL_TEXTURE_2D, image->getTextureID());
    }
    
    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible)
    {
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
        RenderParticles();
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
    
}

void ParticleSystem::DrawShader()
{
    // Allocate the memory for Particle System
    // The number of particle are contained in MAX_PARTICLES macro
    particles = ( Particle* )malloc( sizeof(Particle) * MAX_PARTICLES );
    
    // Start position (0.0, 0.0, 0.0)
    sourcePosition = glm::vec3(0.0, 0.0, 0.0);
    
    program = ProgramManagerObj->ProgramLoad("ParticleSystem", VERTEX_SHADER_PRG, FRAGMENT_SHADER_PRG);
    glUseProgram( program->ProgramID );
    
    TEX                     = GetUniform( program, (char *) "Tex1" );
    worldUniform            = GetUniform( program, (char *) "worldMatrix" );
    viewProjectionUniform   = GetUniform( program, (char *) "viewProjectionMatrix" );
    life                    = GetUniform( program, (char *) "lifeFactor" );
    
}

void ParticleSystem::InitParticles()
{
    // define the type of mesh to use for the particles
    particleMesh        = CreateQuadrilateral();
    float lowestSpeed   = 1.0f;
    float highestSpeed  = 4.0f;
    float rangeSpeed    = 1.0f;
    unsigned int ii     = 0;
    
    for( ii = 0; ii < MAX_PARTICLES; ++ii )
    {
        Particle* particle  = &particles[ ii ];
        particle->transform = mat4();
        particle->pos       = sourcePosition;
        particle->life      = -1.0f;
        particle->transform = glm::translate(particle->transform, particle->pos );
        lowestSpeed         = -2.0;
        highestSpeed        = 2.0f;
        rangeSpeed          = ( highestSpeed - lowestSpeed ) + 1;
        float f             = ( float )( lowestSpeed + ( rangeSpeed * rand() / (RAND_MAX + 1.0f) ) );
        particle->vel.x     = f;
        lowestSpeed         = 4.0;
        highestSpeed        = 8.0f;
        rangeSpeed          = ( highestSpeed - lowestSpeed ) + 1;
        f                   = ( float )( lowestSpeed + ( rangeSpeed * rand() / (RAND_MAX + 1.0f) ) );
        particle->vel.y     = f;
        particle->vel.z     = 0;
    }
}

//--------------------------------------------------
// Update the particles
//--------------------------------------------------
void ParticleSystem::EmitParticles(float elapsedTime )
{
    static float fRotation = 0.0f;
    if(fRotation>360.0){
        fRotation = 0.0;
    }
    
    int spawn   = 0;
    unsigned ii = 0;
    for( ii = 0; ii < MAX_PARTICLES; ++ii )
    {
        Particle* particle = &particles[ ii ];
        
        // Undead
        if(particle->life > 0.0f){
            unsigned int bIsEven    = ( ( ii % 2 ) == 0 ) ? 1 : 0;
            particle->transform     = glm::rotate( particle->transform, ( bIsEven ) ? fRotation : -fRotation, glm::vec3( 0.0, 0.0, 1.0 ) );
            glm::vec3 vel           = particle->vel/100.0f * elapsedTime;
            particle->pos           = particle->pos + vel;
            
            particle->life          -= particle->vel.y * elapsedTime;
            particle->transform     = glm::translate( particle->transform, particle->pos);
        }
        // dead
        else
        {
            // Re-Spawn a max of 4 particles every frame
            if( spawn++ > 4 ){
                continue;
            }
            particle->pos       = sourcePosition;
            particle->life      = MAX_LIFE;
            particle->transform = mat4();
        }
        
        float fScaleFactor = 1.0+(particle->pos.y * 0.25f);
        particle->transform = glm::scale(particle->transform, glm::vec3( fScaleFactor, fScaleFactor, fScaleFactor ));
    }
}

//--------------------------------------------------
// Render the particles
//--------------------------------------------------
void ParticleSystem::RenderParticles()
{
    // Set the shader program
    glUseProgram( program->ProgramID );
    
    // All the particles are using the same texture so it only
    // needs to be set once for all the particles
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture( GL_TEXTURE0 );
    if(image){
    glBindTexture( GL_TEXTURE_2D, image->getTextureID() );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glUniform1i( TEX, 0 );
    glm::mat4 viewProj = *TransformObj->TransformGetModelViewProjectionMatrix();
    
    // Loop through the particles
    unsigned int ii = 0;
    for( ii = 0; ii < MAX_PARTICLES; ++ii )
    {
        // Current particle
        Particle* particle = &particles[ ii ];
        
        // Pointer to the particle mesh
        MeshParticle* pMesh = particleMesh;
        
        // Only draw the particle if it is alive
        if( particle->life > 0.0f )
        {
            // Set the particle transform uniform
            glUniformMatrix4fv( worldUniform, 1, GL_FALSE, ( const GLfloat* )&particle->transform );
            
            // Set view and projection matrices
            glm::mat4 mvp = viewProj * particle->transform ;
            glUniformMatrix4fv( viewProjectionUniform, 1, GL_FALSE, ( const GLfloat* )&mvp );
            
            // Send the remaining life span.
            glUniform1f( life, particle->life / MAX_LIFE );

            // Enable and Set the vertex attributes:- position, texture coords
            glEnableVertexAttribArray( VERTEX_POSITION );
            glEnableVertexAttribArray( TEX_COORD );
            glVertexAttribPointer( VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), &pMesh->vertices->pos );
            glVertexAttribPointer( TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), &pMesh->vertices->texCoord );
            
            // Draw
            glDrawArrays( GL_TRIANGLES, 0, pMesh->vertexCount );
        }
    }
}

void ParticleSystem::TouchEventDown( float x, float y )
{
    Model::TouchEventDown(x,y);
}

void ParticleSystem::TouchEventMove( float x, float y )
{
    Model::TouchEventMove(x,y);
}

void ParticleSystem::TouchEventRelease( float x, float y )
{
    Model::TouchEventRelease(x,y);
}

bool ParticleSystem::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
{
    return false;
}

