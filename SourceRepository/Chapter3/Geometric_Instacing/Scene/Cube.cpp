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
#define VERTEX_SHADER_PRG			( char * )"GeometricInstancingVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"GeometricInstancingFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/GeometricInstancingVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/GeometricInstancingFragment.glsl"
#endif

#define VERTEX_LOCATION 0
#define COLOR_LOCATION 1
#define MATRIX1_LOCATION 2
#define MATRIX2_LOCATION 3
#define MATRIX3_LOCATION 4
#define MATRIX4_LOCATION 5

GLuint vId;
GLuint iId;
GLuint matrixId;
GLuint Vertex_VAO_Id;
int size;

const int dimension    = 10;
static float distance   = 5.0;

// Namespace used
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
//float XOffsetArray[OffsetArraySize];

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


    // Create VBO
 	size = 24*sizeof(float);
    glGenBuffers(1, &vId);
	glBindBuffer( GL_ARRAY_BUFFER, vId );
	glBufferData( GL_ARRAY_BUFFER, size + size, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			size,	cubeVerts );
	glBufferSubData( GL_ARRAY_BUFFER, size,			size,	cubeColors );


    // Create VBO for transformation matrix
    glGenBuffers(1, &matrixId);
	glBindBuffer( GL_ARRAY_BUFFER, matrixId );


    glm::mat4 transformMatrix[dimension][dimension][dimension];
    glBufferData(GL_ARRAY_BUFFER, sizeof(transformMatrix) , 0, GL_DYNAMIC_DRAW);

    // Create IBO
	unsigned short indexSize = sizeof( unsigned short )*36;
    glGenBuffers(1, &iId);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSize, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, indexSize,	cubeIndices );

    glGenVertexArrays(1, &Vertex_VAO_Id);
    glBindVertexArray(Vertex_VAO_Id);

    // Create VBO  and set attribute parameters
	glBindBuffer( GL_ARRAY_BUFFER, vId );
    glEnableVertexAttribArray(VERTEX_LOCATION);
    glEnableVertexAttribArray(COLOR_LOCATION);
    glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribPointer(COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)size);

    // Create VBO for transformation matrix and set attribute parameters
	glBindBuffer( GL_ARRAY_BUFFER, matrixId );
    glEnableVertexAttribArray(MATRIX1_LOCATION);
    glEnableVertexAttribArray(MATRIX2_LOCATION);
    glEnableVertexAttribArray(MATRIX3_LOCATION);
    glEnableVertexAttribArray(MATRIX4_LOCATION);

    glVertexAttribPointer(MATRIX1_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 0));
    glVertexAttribPointer(MATRIX2_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4));
    glVertexAttribPointer(MATRIX3_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 8));
    glVertexAttribPointer(MATRIX4_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 12));

    glVertexAttribDivisor(MATRIX1_LOCATION, 1);
    glVertexAttribDivisor(MATRIX2_LOCATION, 1);
    glVertexAttribDivisor(MATRIX3_LOCATION, 1);
    glVertexAttribDivisor(MATRIX4_LOCATION, 1);

    // Bind IBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );


    // Make sure the VAO is not changed from outside code
    glBindVertexArray(0);

    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::Render()
{
    glEnable( GL_DEPTH_TEST );

    glUseProgram( program->ProgramID );
    RenderCube();
}

void Cube::RenderCube()
{
	glBindBuffer( GL_ARRAY_BUFFER, matrixId );
    glm::mat4* matrixBuf = (glm::mat4*)glMapBufferRange( GL_ARRAY_BUFFER, 0, sizeof(glm::mat4*)*(dimension*dimension*dimension), GL_MAP_WRITE_BIT);
    static float l = 0;
    TransformObj->TransformRotate(l++, 1, 1, 1);
    TransformObj->TransformTranslate(-distance*dimension/4,  -distance*dimension/4, -distance*dimension/4);
    glm::mat4 projectionMatrix  = *TransformObj->TransformGetProjectionMatrix();
    glm::mat4 modelMatrix       = *TransformObj->TransformGetModelMatrix();
    glm::mat4 viewMatrix        = *TransformObj->TransformGetViewMatrix();
    int instance = 0;
    for ( int i = 0; i < dimension; i++ )
    {
        for ( int j = 0; j < dimension; j++ )
        {
            for ( int k = 0; k < dimension; k++ )
            {
                matrixBuf[instance++] = projectionMatrix * viewMatrix * glm::translate(modelMatrix, glm::vec3( i*distance , j*distance, k*distance)) * glm::rotate( modelMatrix, l, glm::vec3(1.0, 0.0, 0.0));
            }
        }
    }
    glUnmapBuffer ( GL_ARRAY_BUFFER );

    glBindVertexArray(Vertex_VAO_Id);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0, dimension*dimension*dimension);
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

