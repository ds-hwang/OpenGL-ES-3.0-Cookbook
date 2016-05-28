#include "CompressedTexture.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"compressedTexVert.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"compressedTexFrag.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/compressedTexVert.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/compressedTexFrag.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

static float rotationX = 0.0;
static float rotationY = 0.0;
static float lastX = 0;
static float lastY = 0;
#define PACE 1.0
static float deltaX = 0;
static float deltaY = 0;
static float DecelerationFactor = 0.5;

float texCoords[8] = { 0.0f, 1.0f,
                        1.0f, 1.0f,
                        0.0f, 0.0f,
                        1.0f, 0.0f
                     };

float quad[12]     ={
                        -1.0f, -1.0f,  0.0f,
                         1.0f, -1.0f,  0.0f,
                        -1.0f, 1.0f, -0.0f,
                         1.0f, 1.0f, -0.0f
                    };

// Namespace used
using std::ifstream;
using std::ostringstream;


/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
CompressedTexture::CompressedTexture( Renderer* parent )
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= CubeType;
    char fname[MAX_PATH]= {""};
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/GLPiFramework/Images/" );
#endif
    strcat( fname, "SmallEarth.pkm" );
    compressImage = new CompressedImage();
    compressImage->loadImage(fname);
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
CompressedTexture::~CompressedTexture()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"square" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void CompressedTexture::InitModel()
{

	if (! ( program = ProgramManagerObj->Program( ( char * )"square" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"square" );
		ProgramManagerObj->AddProgram( program );
	}

	program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);

    /////////// Vertex shader //////////////////////////
	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );

	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
        m = mclose( m );
	}
    
    /////////// Fragment shader //////////////////////////
	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        m = mclose( m );
	}
    
    
    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );
    
    glUseProgram( program->ProgramID );
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    TEX = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "Tex1" );
    
    glActiveTexture (GL_TEXTURE0);
	if (compressImage) {
        glBindTexture(GL_TEXTURE_2D, compressImage->getTextureID());
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void CompressedTexture::Render()
{
    glDisable(GL_CULL_FACE);
	glUseProgram(program->ProgramID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
	TransformObj->TransformPushMatrix();
	TransformObj->TransformRotate(rotationX, 0.0, 1.0, 0.0);
	TransformObj->TransformRotate(rotationY, 1.0, 0.0, 0.0);
   
    glUniform1i(TEX, 0);

    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
	glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, quad);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	TransformObj->TransformPopMatrix();
    
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glDisableVertexAttribArray(VERTEX_POSITION);
	glDisableVertexAttribArray(TEX_COORD);
    if ( deltaX != 0.0 ){
        if( deltaX > 0.0 ){
            deltaX -= DecelerationFactor;
        }
        else{
            deltaX += DecelerationFactor;
        }
        rotationX += deltaX;
    }

    if ( deltaY != 0.0 ){
        if( deltaY > 0.0 ){
            deltaY -= DecelerationFactor;
        }
        else{
            deltaY += DecelerationFactor;
        }
        rotationY += deltaY;
    }
}

void CompressedTexture::TouchEventDown( float x, float y )
{
    lastX = x;
    lastY = y;
}

void CompressedTexture::TouchEventMove( float x, float y )
{
    rotationX += (x - lastX) * PACE;
    rotationY += (y - lastY) * PACE;
    lastX = x;
    lastY = y;
}

void CompressedTexture::TouchEventRelease( float x, float y )
{
    deltaX = x - lastX;
    deltaY = y - lastY;
}



