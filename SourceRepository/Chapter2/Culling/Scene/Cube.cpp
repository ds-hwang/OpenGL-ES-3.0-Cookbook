#include "Cube.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
//#include "Transform.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"CubeVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"CubeFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/CubeVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/CubeFragment.glsl"
#endif

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

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Cube::Cube( Renderer* parent )
{
	if (!parent)
		return;

	//RenderObj			= parent;
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
        freeCache( m );
	}

	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        freeCache( m );
	}

    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
	MVP          = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MODELVIEWPROJECTIONMATRIX");
	attribVertex = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
	attribColor  = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    
    return;
}
static bool toogle = true;

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::Render()
{
	glEnable( GL_CULL_FACE  );   // Enable the culling
    if (toogle){
        glCullFace ( GL_FRONT ); // Culls geometries front face
    }
    else{
        glCullFace ( GL_BACK );  // Culls geometries back face
    }

	static float k = 0;
    glUseProgram( program->ProgramID );

	TransformObj->TransformRotate(k++, 1, 1, 1);
	TransformObj->TransformPushMatrix();

    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    
	TransformObj->TransformPopMatrix();
    glEnableVertexAttribArray(attribVertex);
    glEnableVertexAttribArray(attribColor);
    glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, vertexBuffer);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, colorBufferData);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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