#include "ParticleSystem.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG_EMIT			( char * )"TFUpdateVert.glsl"
#define FRAGMENT_SHADER_PRG_EMIT		( char * )"TFUpdateFrag.glsl"
#define VERTEX_SHADER_PRG_DRAW			( char * )"TFDrawVert.glsl"
#define FRAGMENT_SHADER_PRG_DRAW		( char * )"TFDrawFrag.glsl"
#else
#define VERTEX_SHADER_PRG_EMIT			( char * )"shader/TFUpdateVert.glsl"
#define FRAGMENT_SHADER_PRG_EMIT		( char * )"shader/TFUpdateFrag.glsl"
#define VERTEX_SHADER_PRG_DRAW			( char * )"shader/TFDrawVert.glsl"
#define FRAGMENT_SHADER_PRG_DRAW		( char * )"shader/TFDrawFrag.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

#define GetUniform ProgramManagerObj->ProgramGetUniformLocation

// Namespace used
using std::ifstream;
using std::ostringstream;
using namespace glm;
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
    if(image){
        glBindTexture( GL_TEXTURE_2D, image->getTextureID() );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    //Create3DNoiseTexture ( 128, 50.0 );
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
ParticleSystem::~ParticleSystem()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( (char*)"TFEmit" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

void ParticleSystem::UpdateShader()
{
    program = ProgramManagerObj->ProgramLoad((char*)"TFEmit", VERTEX_SHADER_PRG_EMIT, FRAGMENT_SHADER_PRG_EMIT);

    glUseProgram( program->ProgramID );
    
    emitProgramObject = program->ProgramID;
    
    const char *feedbackVaryings[5] =
    {
        "position",
        "velocity",
        "size",
        "currentTime",
        "lifeTime"
    };
    
    // Set the vertex shader outputs as transform feedback varyings
    glTransformFeedbackVaryings ( emitProgramObject, 5, feedbackVaryings, GL_INTERLEAVED_ATTRIBS );
    
    // Link program must occur after calling glTransformFeedbackVaryings
    glLinkProgram ( emitProgramObject );
    
    // Get the uniform locations - this also needs to happen after glLinkProgram is called again so
    // that the uniforms that output to varyings are active
    emitTimeLoc = GetUniform ( program, (char*)"time" );
    emitEmissionRateLoc = GetUniform ( program, (char*)"emissionRate" );
    emitNoiseSamplerLoc = GetUniform ( program, (char*)"noiseTex" );
}

void ParticleSystem::DrawShader()
{
    program = ProgramManagerObj->ProgramLoad((char*)"TFDraw", VERTEX_SHADER_PRG_DRAW, FRAGMENT_SHADER_PRG_DRAW);

    glUseProgram( program->ProgramID );
    
    MVP = GetUniform( program, ( char* )"ModelViewProjectMatrix" );

    // Load the shaders and get a linked program object
    drawProgramObject = program->ProgramID;
    
    // Get the uniform locations
    drawTimeLoc         = GetUniform ( program, (char*)"time" );
    drawColorLoc        = GetUniform ( program, (char*)"color" );
    drawAccelerationLoc = GetUniform ( program, (char*)"acceleration" );
    samplerLoc          = GetUniform ( program, (char*)"tex" );
}

void ParticleSystem::InitParticles()
{
    
    time        = 0.0f;
    curSrcIndex = 0;
    textureId   = image->getTextureID();
    
    if ( textureId <= 0 ){
        return;
    }
    
    // Create a 3D noise texture for random values
    noiseTextureId = Create3DNoiseTexture ( 128, 50.0 );
    Particle particleData[ NUM_PARTICLES ];
    // Initialize particle data
    for ( int i = 0; i < NUM_PARTICLES; i++ )
    {
        Particle *particle      = &particleData[i];
        particle->position[0]   = 0.0f;
        particle->position[1]   = 0.0f;
        particle->velocity[0]   = 0.0f;
        particle->velocity[1]   = 0.0f;
        particle->size          = 0.0f;
        particle->curtime       = 0.0f;
        particle->lifetime      = 0.0f;
    }
    
    
    // Create the particle VBOs
    glGenBuffers ( 2, &particleVBOs[0] );
    
    for ( int i = 0; i < 2; i++ )
    {
        glBindBuffer ( GL_ARRAY_BUFFER, particleVBOs[i] );
        glBufferData ( GL_ARRAY_BUFFER, sizeof ( Particle ) * NUM_PARTICLES, particleData, GL_DYNAMIC_COPY );
    }
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ParticleSystem::InitModel()
{
    UpdateShader();
    DrawShader();
    InitParticles();
    Model::InitModel();
    return;
}

/*!
	Set the attribute in the PingPong VBO

	\param[in] None.
	\return None

*/
void ParticleSystem::SetupVertexAttributes ( GLuint vboID )
{
    glBindBuffer ( GL_ARRAY_BUFFER, vboID );
    glVertexAttribPointer ( ATTRIBUTE_POSITION, 2, GL_FLOAT,
                           GL_FALSE, sizeof ( Particle ),
                           ( const void * ) NULL );
    
    glVertexAttribPointer ( ATTRIBUTE_VELOCITY, 2, GL_FLOAT,
                           GL_FALSE, sizeof ( Particle ),
                           ( const void * ) offsetof ( Particle, velocity[0] ) );
    
    glVertexAttribPointer ( ATTRIBUTE_SIZE, 1, GL_FLOAT,
                           GL_FALSE, sizeof ( Particle ),
                           ( const void * ) offsetof ( Particle, size ) );
    
    glVertexAttribPointer ( ATTRIBUTE_CURTIME, 1, GL_FLOAT,
                           GL_FALSE, sizeof ( Particle ),
                           ( const void * ) offsetof ( Particle, curtime ) );
    
    glVertexAttribPointer ( ATTRIBUTE_LIFETIME, 1, GL_FLOAT,
                           GL_FALSE, sizeof ( Particle ),
                           ( const void * ) offsetof ( Particle, lifetime ) );
    
    glEnableVertexAttribArray ( ATTRIBUTE_POSITION );
    glEnableVertexAttribArray ( ATTRIBUTE_VELOCITY );
    glEnableVertexAttribArray ( ATTRIBUTE_SIZE );
    glEnableVertexAttribArray ( ATTRIBUTE_CURTIME );
    glEnableVertexAttribArray ( ATTRIBUTE_LIFETIME );
}

void ParticleSystem::Update ()
{
    static clock_t lastTime = clock();
    clock_t currentTime     = clock();
    float deltaTime         = (currentTime - lastTime)/(CLOCKS_PER_SEC*0.10);
    lastTime                = currentTime;
    time                    += deltaTime;
    
    EmitParticles ( deltaTime );
}

void ParticleSystem::Render()
{
    Update ();
    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible){
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
        TransformObj->TransformPopMatrix(); // Local Level
    }
    
    RenderParticles();
    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
    
}

void ParticleSystem::RenderParticles()
{
    // Ensure that the OpenGL ES GL server is blocked until
    // the transform feedback vertex output variable are not captured.
    glWaitSync ( emitSync, 0, GL_TIMEOUT_IGNORED );
    glDeleteSync ( emitSync );
    
    // Use the program object
    glUseProgram ( drawProgramObject );
    
    // Load the VBO and vertex attributes
    SetupVertexAttributes ( particleVBOs[ curSrcIndex ] );
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    
    // Set uniforms
    glUniform1f ( drawTimeLoc, time );
    glUniform4f ( drawColorLoc, 1.0f, 1.0f, 1.0f, 1.0f );
    glUniform2f ( drawAccelerationLoc, 0.0f, ACCELERATION );
    
    // Blend particles
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE );
    
    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, textureId );
    
    // Set the sampler texture unit to 0
    glUniform1i ( samplerLoc, 0 );
    
    glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
}

void ParticleSystem::EmitParticles(float deltaTime )
{
    //UserData *userData = esContext->userData;
    GLuint srcVBO = particleVBOs[ curSrcIndex ];
    GLuint dstVBO = particleVBOs[ ( curSrcIndex + 1 ) % 2 ];
    
    glUseProgram ( emitProgramObject );
    
    SetupVertexAttributes ( srcVBO );
    
    // Set transform feedback buffer
    glBindBuffer ( GL_TRANSFORM_FEEDBACK_BUFFER, dstVBO );
    glBindBufferBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 0, dstVBO );
    
    // Turn off rasterization - we are not drawing
    glEnable ( GL_RASTERIZER_DISCARD );
    
    // Set uniforms
    glUniform1f ( emitTimeLoc, time );
    glUniform1f ( emitEmissionRateLoc, EMISSION_RATE );
    
    // Bind the 3D noise texture
    glActiveTexture ( GL_TEXTURE1 );
    glBindTexture ( GL_TEXTURE_3D, noiseTextureId );
    glUniform1i ( emitNoiseSamplerLoc, 1 );
    
    // Emit particles using transform feedback
    glBeginTransformFeedback ( GL_POINTS );
    glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
    glEndTransformFeedback();
    
    // Use fences and create a sync object and make sure that
    // transform feedback results are completed before the draw that uses them.
    emitSync = glFenceSync ( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
    
    // Restore state
    glDisable ( GL_RASTERIZER_DISCARD );
    glUseProgram ( 0 );
    glBindBufferBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0 );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    glBindTexture ( GL_TEXTURE_3D, 0 );
    
    // Ping pong the buffers
    curSrcIndex = ( curSrcIndex + 1 ) % 2;
}

unsigned int ParticleSystem::Create3DNoiseTexture ( int textureSize, float frequency )
{
    GLuint textureId;
    GLfloat *texBuf = ( GLfloat * ) malloc ( sizeof ( GLfloat ) * textureSize * textureSize * textureSize ) ;
    GLubyte *uploadBuf = ( GLubyte * ) malloc ( sizeof ( GLubyte ) * textureSize * textureSize * textureSize ) ;
    int x, y, z;
    int index = 0;
    float min = 1000;
    float max = -1000;
    float range;
    float noiseVal;
    
    for ( z = 0; z < textureSize; z++ ){
        for ( y = 0; y < textureSize; y++ ){
            for ( x = 0; x < textureSize; x++ ){
                noiseVal = glm::perlin(glm::vec3( ( random() % 32768 ) / 32768.0f, ( random() % 32768 ) / 32768.0f, ( random() % 32768 ) / 32768.0f));
                if ( noiseVal < min ){
                    min = noiseVal;
                }
                
                if ( noiseVal > max ){
                    max = noiseVal;
                }
                
                texBuf[ index++ ] = noiseVal;
            }
        }
    }
    
    // Normalize to the [0, 1] range
    range = ( max - min );
    index = 0;
    
    for ( z = 0; z < textureSize; z++ ){
        for ( y = 0; y < textureSize; y++ ){
            for ( x = 0; x < textureSize; x++ ){
                float noiseVal = texBuf[index];
                noiseVal = ( noiseVal - min ) / range;
                uploadBuf[index++] = ( GLubyte ) ( noiseVal * 255.0f );
            }
        }
    }
    
    glGenTextures ( 1, &textureId );
    glBindTexture ( GL_TEXTURE_3D, textureId );
    glTexImage3D ( GL_TEXTURE_3D, 0, GL_R8, textureSize, textureSize, textureSize, 0,
                  GL_RED, GL_UNSIGNED_BYTE, uploadBuf );
    
    glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
    glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT );
    
    glBindTexture ( GL_TEXTURE_3D, 0 );
    
    free ( texBuf );
    free ( uploadBuf );
    
    return textureId;
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

// Discuss in There More section.

bool ParticleSystem::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
{
    return false;
}


