#include "SkyBox.h"

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

// Namespace used
using std::ifstream;
using std::ostringstream;

float side = 20.0f;
float semiSide = side * 0.5f;

float vertexArray[24*3] = {
    // Front Face
    -semiSide, -semiSide, semiSide,
    semiSide, -semiSide, semiSide,
    semiSide,  semiSide, semiSide,
    -semiSide,  semiSide, semiSide,
    
    // Right Face
    semiSide, -semiSide, semiSide,
    semiSide, -semiSide, -semiSide,
    semiSide,  semiSide, -semiSide,
    semiSide,  semiSide, semiSide,
    
    // Back Face
    -semiSide, -semiSide, -semiSide,
    -semiSide,  semiSide, -semiSide,
    semiSide,  semiSide, -semiSide,
    semiSide, -semiSide, -semiSide,
    
    // Left Face
    -semiSide, -semiSide, semiSide,
    -semiSide,  semiSide, semiSide,
    -semiSide,  semiSide, -semiSide,
    -semiSide, -semiSide, -semiSide,
    
    // Bottom Face
    -semiSide, -semiSide, semiSide,
    -semiSide, -semiSide, -semiSide,
    semiSide, -semiSide, -semiSide,
    semiSide, -semiSide, semiSide,
    
    // Top Face
    -semiSide,  semiSide, semiSide,
    semiSide,  semiSide, semiSide,
    semiSide,  semiSide, -semiSide,
    -semiSide,  semiSide, -semiSide
};

float normalArray[24*3] = {
    // Front
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    // Right
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    // Back
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    // Left
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    // Bottom
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    // Top
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

float colorArray[24*3] = {
    // Front
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    // Right
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    // Back
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    // Left
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    // Bottom
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    // Top
    0.5f, 1.0f, 0.5f,
    0.5f, 1.0f, 0.5f,
    0.5f, 1.0f, 0.5f,
    0.5f, 1.0f, 0.5f
};


float textureArray[24*2] = {
    // Front
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    // Right
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    // Back
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    // Left
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    // Bottom
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    // Top
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

GLushort indiceArray[] = {
    0,1,2,0,2,3,
    4,5,6,4,6,7,
    8,9,10,8,10,11,
    12,13,14,12,14,15,
    16,17,18,16,18,19,
    20,21,22,20,22,23
};


GLuint vId;
GLuint iId;

int vSize;
int nSize;
int cSize;
int tSize;
unsigned short iSize;

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
	vSize = 6 * 4 * 3 * sizeof( float ); // 6 faces, 4 vertex, 3 coordinate per vertex.
	nSize = 6 * 4 * 3 * sizeof( float );
	cSize = 6 * 4 * 3 * sizeof( float );
	tSize = 6 * 4 * 2 * sizeof( float );
    iSize = 6 * 6 * sizeof( unsigned short ); // 6 faces, (2*3) 2 triangle per face, each triangle has 3 index
    
    glGenBuffers(1, &vId);
    
	glBindBuffer( GL_ARRAY_BUFFER, vId );
	glBufferData( GL_ARRAY_BUFFER, vSize + nSize + cSize + tSize, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			vSize,	vertexArray );
	glBufferSubData( GL_ARRAY_BUFFER, vSize,			nSize,	normalArray );
	glBufferSubData( GL_ARRAY_BUFFER, vSize+nSize,			cSize,	colorArray );
	glBufferSubData( GL_ARRAY_BUFFER, vSize+nSize+cSize,	tSize,	textureArray );
    
    glGenBuffers(1, &iId);
	glBindBuffer( GL_ARRAY_BUFFER, iId );
	glBufferData( GL_ARRAY_BUFFER, iSize, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, iSize,	indiceArray );
	
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
	//PROGRAM *program                    = NULL;
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
    
    loadCubeMap();
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
    
	static float k = 0;
    glUseProgram( program->ProgramID );
    
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );

    glBindBuffer( GL_ARRAY_BUFFER, vId );
    char attribVertex = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
    if (attribVertex>=0)
    {
        glEnableVertexAttribArray(attribVertex);
        glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    char attribColor = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    if (attribColor>=0){
        glEnableVertexAttribArray(attribColor);
        glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vSize+nSize));
    }

	if (image){
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, image->getTextureID());
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
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

void Skybox::loadCubeMap()
{
    glActiveTexture(GL_TEXTURE1);
	char fname[MAX_PATH]= {""};
    image = new PngImage();
    image->loadImage(creatPath(fname, (char*)"Right.png"), true, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    image->loadImage(creatPath(fname, (char*)"Left.png"), false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    image->loadImage(creatPath(fname, (char*)"Top.png"), false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    image->loadImage(creatPath(fname, (char*)"Bottom.png"), false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    image->loadImage(creatPath(fname, (char*)"Middle.png"), false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    image->loadImage(creatPath(fname, (char*)"ExtremeTop.png"), false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    char uniformTex = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "CubeMapTexture" );
    if (uniformTex >= 0) {
        glUniform1i(uniformTex, 1);
    }
}