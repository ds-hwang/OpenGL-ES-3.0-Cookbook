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
#define VERTEX_SHADER_PRG			( char * )"SimpleInstancingVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"SimpleInstancingFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/SimpleInstancingVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/SimpleInstancingFragment.glsl"
#endif

#define VERTEX_LOCATION 0
#define COLOR_LOCATION 1
#define XOFFSET_LOCATION 2
GLuint vId;
GLuint iId;
GLuint XOffset;
GLuint Vertex_VAO_Id;
int size;

// Namespace used
using std::ifstream;
using std::ostringstream;
bool Animate = true;
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

// 36[ indices
GLushort cubeIndices[] = {
    0,3,1, 3,2,1,
    7,4,6, 4,5,6,
    4,0,5, 0,1,5,
    3,7,2, 7,6,2,
    1,2,5, 2,6,5,
    3,0,7, 0,4,7
};

const int OffsetArraySize = 10;
float XOffsetArray[OffsetArraySize];

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

    float multiple = 3.0f;
    for(int i=0;i<OffsetArraySize;i++){
        XOffsetArray[i] = multiple * i;
    }

	LOGI("gl2: Cube constructor");

    // Create name buffer object ID
	size = 24*sizeof(float);
    glGenBuffers(1, &vId);
    
    // Create VBO
	glBindBuffer( GL_ARRAY_BUFFER, vId );
	glBufferData( GL_ARRAY_BUFFER, size + size, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			size,	cubeVerts );
	glBufferSubData( GL_ARRAY_BUFFER, size,			size,	cubeColors );

    
    glGenBuffers(1, &XOffset);
    glBindBuffer(GL_ARRAY_BUFFER, XOffset);
	glBufferData( GL_ARRAY_BUFFER, sizeof(XOffsetArray), 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(XOffsetArray),	XOffsetArray );
    
    // Create IBO
	unsigned short indexSize = sizeof( unsigned short )*36;
    glGenBuffers(1, &iId);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSize, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, indexSize,	cubeIndices );
    
    
    glGenVertexArrays(1, &Vertex_VAO_Id);
    glBindVertexArray(Vertex_VAO_Id);
    
    glBindBuffer( GL_ARRAY_BUFFER, vId );
    glEnableVertexAttribArray(VERTEX_LOCATION);
    glEnableVertexAttribArray(COLOR_LOCATION);
    glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribPointer(COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)size);

    glBindBuffer( GL_ARRAY_BUFFER, XOffset );
    glEnableVertexAttribArray(XOFFSET_LOCATION);
    glVertexAttribPointer(XOFFSET_LOCATION, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(XOFFSET_LOCATION,  1);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );

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

    //Initialize Vertex and Fragment shader
	program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);

    // Compile Vertex shader
	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
        freeCache( m );
	}

    // Compile Fragment shader
	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        freeCache( m );
	}

    // Link program
    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
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
	//static float k      = -1.0;
    glEnable( GL_DEPTH_TEST );

    glUseProgram( program->ProgramID );
	TransformObj->TransformTranslate(0.0,  0.0, -50.0);
    static int j = 0;
    TransformObj->TransformRotate(j++, 1, 1, 1);
    RenderCube();
}

void Cube::RenderCube()
{
	char uniform = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
	if ( uniform >= 0 ){
		glUniformMatrix4fv( uniform, 1, GL_FALSE,(float*)TransformObj->TransformGetModelViewProjectionMatrix() );
	}
    glBindVertexArray(Vertex_VAO_Id);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0, sizeof(XOffsetArray)/sizeof(GL_FLOAT));
}

/*!
 This function handle Touch event down action.
 
 \param[in] x and y screen pixel position.
 
 \return None.
 */
void Cube::TouchEventDown( float x, float y )
{
	Animate = !Animate;
}

