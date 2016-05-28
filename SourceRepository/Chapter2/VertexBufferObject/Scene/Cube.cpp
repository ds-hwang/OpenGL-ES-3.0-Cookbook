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

bool Animate            = true;
static int dimension    = 10;
static float distance   = 5.0;
static bool op          = true;
GLuint vId              = 0;
GLuint iId              = 0;
int size                = 0;

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
	if (! ( program = ProgramManagerObj->Program( (char *)"Cube" ))){
		program = ProgramManagerObj->ProgramInit( (char *)"Cube" );
		ProgramManagerObj->AddProgram( program );
	}

	program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
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
    mvp    = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    attribVertex   = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
    attribColor    = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    return;
}
static float k = 0;

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::Render()
{
    glEnable( GL_DEPTH_TEST );
    glUseProgram( program->ProgramID );
    if(distance > 5)
        op = true;
    if(distance < 2.0)
        op = false;
    
    if (Animate){
        if(op)
            distance -= 0.1;
        else
            distance += 0.1;
    }

    TransformObj->TransformTranslate(0.0f, 0.0f, -100);
    TransformObj->TransformRotate(k++, 1, 1, 1);
    RenderCubeOfCubes();
}

void Cube::RenderCube()
{
    
	glUniformMatrix4fv( mvp, 1, GL_FALSE,(float*)TransformObj->TransformGetModelViewProjectionMatrix() );
    
    
    glEnableVertexAttribArray(attribColor);
    glEnableVertexAttribArray(attribVertex);
    
    glBindBuffer( GL_ARRAY_BUFFER, vId );
    glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)size);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void Cube::RenderCubeOfCubes()
{
	TransformObj->TransformTranslate(-distance*dimension/2,  -distance*dimension/2, -distance*dimension/2);
	for (int i = 0; i<dimension; i++){
		TransformObj->TransformTranslate(distance,  0.0, 0.0);
		TransformObj->TransformPushMatrix();
		for (int j = 0; j<dimension; j++){
			TransformObj->TransformTranslate(0.0,  distance, 0.0);
            TransformObj->TransformPushMatrix();
            for (int j = 0; j<dimension; j++){
                TransformObj->TransformTranslate(0.0,  0.0, distance);
                RenderCube();
            }
            TransformObj->TransformPopMatrix();
		}
		TransformObj->TransformPopMatrix();
	}
    
}

