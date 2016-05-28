#include "Cube.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
//#include "Transform.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"CubeVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"CubeFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/CubeVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/CubeFragment.glsl"
#endif

//#ifdef __IPHONE_4_0
//#define VERTEX_SHADER_PRG			( char * )"testVertex.glsl"
//#define FRAGMENT_SHADER_PRG			( char * )"testFragment.glsl"
//#else
//#define VERTEX_SHADER_PRG			( char * )"shader/testVertex.glsl"
//#define FRAGMENT_SHADER_PRG			( char * )"shader/testFragment.glsl"
//#endif

// Namespace used
using std::ifstream;
using std::ostringstream;

// Global Object Declaration
     GLfloat  cubeVerts[][3] = {
			-1, -1, 1 , // V0
			-1, 1,  1 , // V1
			1,  1, 1 ,  // V2
			1,  -1,  1 ,// V3 
			-1, -1, -1 ,// V4
			-1, 1,  -1 ,// V5
			1,  1, -1 , // V6
			1,  -1,  -1 // V7
        };

        GLfloat  cubeColors[][3] = {
            {  0.0,  0.0,  0.0 }, //0
            {  0.0,  0.0,  1.0 }, //1
            {  0.0,  1.0,  0.0 }, //2
            {  0.0,  1.0,  1.0 }, //3
            {  1.0,  0.0,  0.0 }, //4
            {  1.0,  0.0,  1.0 }, //5
            {  1.0,  1.0,  0.0 }, //6
            {  1.0,  1.0,  1.0 }, //7
        };

	GLushort cubeIndices[] = {0,3,1, 3,2,1,   // 36 of indices
                     7,4,6, 4,5,6,
                     4,0,5, 0,1,5,
                     3,7,2, 7,6,2,
                     1,2,5, 2,6,5,
                     3,0,7, 0,4,7};

static const GLfloat vertexBuffer[108] = {
			-1, -1, 1 , // V0
			1,  -1,  1 ,// V3 
			-1, 1,  1 , // V1

			1,  -1,  1 ,// V3 
			1,  1, 1 ,  // V2
			-1, 1,  1 , // V1

			1,  -1,  -1, // V7
			-1, -1, -1 ,// V4
			1,  1, -1 , // V6

			-1, -1, -1 ,// V4
			-1, 1,  -1 ,// V5
			1,  1, -1 , // V6
			
			-1, -1, -1 ,// V4
			-1, -1, 1 , // V0
			-1, 1,  -1 ,// V5
			
			-1, -1, 1 , // V0
			-1, 1,  1 , // V1
			-1, 1,  -1 ,// V5

			1,  -1,  1 ,// V3 
			1,  -1,  -1, // V7
			1,  1, 1 ,  // V2

			1,  -1,  -1, // V7
			1,  1, -1 , // V6
			1,  1, 1 ,  // V2
			
			-1, 1,  1 , // V1
			1,  1, 1 ,  // V2
			-1, 1,  -1 ,// V5
			
			1,  1, 1 ,  // V2
			1,  1, -1 , // V6
			-1, 1,  -1 ,// V5

			1,  -1,  1 ,// V3 
			-1, -1, 1 , // V0
			1,  -1,  -1, // V7

			-1, -1, 1 , // V0
			-1, -1, -1 ,// V4
			1,  -1,  -1 // V7
};

static const GLfloat colorBufferData[108] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};
GLuint vId;
GLuint iId;
int size;
bool toogle = false;
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Cube::Cube( Renderer* parent )
{
	if (!parent)
		return;

	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= CubeType;

	LOGI("gl2: Cube constructor");
	
	size = 24*sizeof(float);
    glGenBuffers(1, &vId);

	glBindBuffer( GL_ARRAY_BUFFER, vId );
	glBufferData( GL_ARRAY_BUFFER, size + size, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			size,	cubeVerts );
	glBufferSubData( GL_ARRAY_BUFFER, size,			size,	cubeColors );

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
Cube::~Cube()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"Cube" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
	glDeleteBuffers(1, &vId);
	glDeleteBuffers(1, &iId);
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::InitModel()
{
	program                             = NULL;
	ProgramManager* ProgramManagerObj   = MapRenderHandler->RendererProgramManager();
	Transform*	TransformObj            = MapRenderHandler->RendererTransform();


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
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::Render()
{
	static float k,j,l = 0;
    glEnable( GL_DEPTH_TEST );
    
	Transform*	TransformObj = MapRenderHandler->RendererTransform();
	TransformObj->TransformPushMatrix();

	if ( MapRenderHandler->isPerspectiveView() ){
 		TransformObj->TransformTranslate(0, 0, -25);
	}
    
    TransformObj->TransformRotate(k++, 0, 1, 0);

    TransformObj->TransformTranslate( 0, 0, -3);
    RenderCubeVBO();
    TransformObj->TransformTranslate( 0, 0, -3);
    RenderCubeVBO();
    TransformObj->TransformTranslate( 0, 0, -3);
    RenderCubeVBO();
    TransformObj->TransformTranslate( 0, 0, -3);
    RenderCubeVBO();
    TransformObj->TransformTranslate( 0, 0, -3);
    RenderCubeVBO();
	TransformObj->TransformPopMatrix();
}

void Cube::RenderCubeVBO(){
    glUseProgram(program->ProgramID);
	char uniform = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    
	if ( uniform >= 0 )
	{
		glUniformMatrix4fv( uniform, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	}
    
    char attribColor = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    
    if (attribColor>=0){
        //LOGI("as: Color");
        glEnableVertexAttribArray(attribColor);
        glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, colorBufferData);
    }
    char attribVertex = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
    if (attribVertex>=0)
    {
        //LOGI("as: Ver");
        glEnableVertexAttribArray(attribVertex);
        glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, vertexBuffer);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisableVertexAttribArray(attribVertex);
    glDisableVertexAttribArray(attribColor);
}


void Cube::TouchEventDown( float x, float y )
{
    toogle = !toogle;
}

void Cube::TouchEventMove( float x, float y )
{
}

void Cube::TouchEventRelease( float x, float y )
{
}
