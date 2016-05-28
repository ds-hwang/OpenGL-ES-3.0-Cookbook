#include "Skybox.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "PngImage.h"
#include "constant.h"

using namespace namespaceimage;

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"CubeMapVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"CubeMapFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/CubeMapVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/CubeMapFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

static float k = 0;


// Namespace used
using std::ifstream;
using std::ostringstream;
unsigned int idx = 0;
// Global Object Declaration
GLfloat  cubeVerts[][3] = {
	-1,-1, 1 , // V0
	-1, 1, 1 , // V1
	 1, 1, 1 , // V2
	 1,-1, 1 , // V3
	-1,-1,-1 , // V4
	-1, 1,-1 , // V5
	 1, 1,-1 , // V6
	 1,-1,-1   // V7
};

GLushort cubeIndices[] = {
    0,3,1, 3,2,1,   // 36 of indices
    7,4,6, 4,5,6,
    4,0,5, 0,1,5,
    3,7,2, 7,6,2,
    1,2,5, 2,6,5,
    3,0,7, 0,4,7
};

GLuint vId;
GLuint iId;
int size;

/*!
 Simple Default Constructor
 
 \param[in] None.
 \return None
 
 */
Skybox::Skybox( Renderer* parent )
{
	if (!parent)
		return;

	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= CubeType;
    image               = NULL;
	
	size = 24*sizeof(float);
    glGenBuffers(1, &vId);

	glBindBuffer( GL_ARRAY_BUFFER, vId );
	glBufferData( GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			size,	cubeVerts );

	unsigned short indexSize = sizeof( unsigned short )*36;
    glGenBuffers(1, &iId);
	glBindBuffer( GL_ARRAY_BUFFER, iId );
	glBufferData( GL_ARRAY_BUFFER, indexSize, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, indexSize,	cubeIndices );
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

}


/*!
 Simple Destructor
 
 \param[in] None.
 \return None
 
 */
Skybox::~Skybox()
{
	//PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"Cube" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
	glDeleteBuffers(1, &vId);
	glDeleteBuffers(1, &iId);
    glDeleteTextures(1, &image->getTextureID());
    delete image;
}

/*!
 Initialize the scene, reserve shaders, compile and cache program
 
 \param[in] None.
 \return None
 
 */
void Skybox::InitModel()
{
	ProgramManager* ProgramManagerObj   = RendererHandler->RendererProgramManager();
	Transform*	TransformObj            = RendererHandler->RendererTransform();


	if (! ( program = ProgramManagerObj->Program( ( char * )"Cube" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"Cube" );
		ProgramManagerObj->AddProgram( program );
	}

	program->VertexShader		= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);

	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);

    //////////////////////////////////////////////////////
    /////////// Vertex shader //////////////////////////
    //////////////////////////////////////////////////////
	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );

	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
	}
	m = mclose( m );

	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
	}

	m = mclose( m );

    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MVP" );
    createCubeMap();
    return;
}

/*!
 Initialize the scene, reserve shaders, compile and cache program
 
 \param[in] None.
 \return None
 
 */
void Skybox::Render()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glUseProgram( program->ProgramID );
	TransformObj->TransformPushMatrix();
	TransformObj->TransformScale(10.0f, 10.0f, 10.0f);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	TransformObj->TransformPopMatrix();
    
    glBindBuffer( GL_ARRAY_BUFFER, vId );
    glEnableVertexAttribArray(VERTEX_POSITION);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
}

char* Skybox::creatPath(char* fname, char* fileName) {
    
    char path[MAX_PATH]= {""};
    #ifdef __IPHONE_4_0
        GLUtils::extractPath( getenv( "FILESYSTEM" ), path );
    #else
        strcpy( path, "/sdcard/Images/skybox/" );
    #endif
    memset( fname, 0, MAX_PATH);
    strcpy( fname, path );
    strcat( fname, fileName );
    return fname;
}


void Skybox::createCubeMap()
{
    glActiveTexture(GL_TEXTURE1);
	char fname[MAX_PATH]= {""};
    image = new PngImage();
    image->loadImage(creatPath(fname, (char*)"Right.png"),  true,  GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    image->loadImage(creatPath(fname, (char*)"Left.png"),   false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    image->loadImage(creatPath(fname, (char*)"Top.png"),    false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    image->loadImage(creatPath(fname, (char*)"Bottom.png"), false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    image->loadImage(creatPath(fname, (char*)"Front.png"),  false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    image->loadImage(creatPath(fname, (char*)"Back.png"),   false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    char uniformTex = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "CubeMapTexture" );
    if (uniformTex >= 0) {
        glUniform1i(uniformTex, 1);
    }
}
