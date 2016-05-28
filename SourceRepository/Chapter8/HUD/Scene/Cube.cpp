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

bool useDrawElementAPI = true;
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
    {  0,  0,  0 }, //0
    {  0,  0,  1 }, //1
    {  0,  1,  0 }, //2
    {  0,  1,  1 }, //3
    {  1,  0,  0 }, //4
    {  1,  0,  1 }, //5
    {  1,  1,  0 }, //6
    {  1,  1,  1 }, //7
};

GLushort cubeIndices[] = {0,3,1, 3,2,1,   // 36 of indices
                 7,4,6, 4,5,6,
                 4,0,5, 0,1,5,
                 3,7,2, 7,6,2,
                 1,2,5, 2,6,5,
                 3,0,7, 0,4,7};

static const GLfloat vertexBuffer[][3] = {
    {-1,-1, 1}, { 1,-1, 1}, {-1, 1, 1}, // V0-V3-V1
    { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1}, // V3-V2-V1
    { 1,-1,-1}, {-1,-1,-1}, { 1, 1,-1}, // V7-V4-V6
    {-1,-1,-1}, {-1, 1,-1}, { 1, 1,-1}, // V4-V5-V6
    {-1,-1,-1}, {-1,-1, 1}, {-1, 1,-1}, // V4-V0-V5
    {-1,-1, 1}, {-1, 1, 1}, {-1, 1,-1}, // V0-V1-V5
    { 1,-1, 1}, { 1,-1,-1}, { 1, 1, 1}, // V3-V7-V2
    { 1,-1,-1}, { 1, 1,-1}, { 1, 1, 1}, // V7-V6-V2
    {-1, 1, 1}, { 1, 1, 1}, {-1, 1,-1}, // V1-V2-V5
    { 1, 1, 1}, { 1, 1,-1}, {-1, 1,-1}, // V2-V6-V5
    { 1,-1, 1}, {-1,-1, 1}, { 1,-1,-1}, // V3-V0-V7
    {-1,-1, 1}, {-1,-1,-1}, { 1,-1,-1}  // V0-V4-V7
};
static const GLfloat colorBufferData[][3] = {
	{1,0,0}, {1,0,0}, {1,0,0},
	{1,0,0}, {1,0,0}, {1,0,0},
	{1,1,0}, {1,1,0}, {1,1,0},
	{1,1,0}, {1,1,0}, {1,1,0},
	{0,1,0}, {0,1,0}, {0,1,0},
	{0,1,0}, {0,1,0}, {0,1,0},
	{0,1,1}, {0,1,1}, {0,1,1},
	{0,1,1}, {0,1,1}, {0,1,1},
	{0,0,1}, {0,0,1}, {0,0,1},
	{0,0,1}, {0,0,1}, {0,0,1},
	{1,0,1}, {1,0,1}, {1,0,1},
	{1,0,1}, {1,0,1}, {1,0,1}
};

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Cube::Cube( Renderer* parent )
{
	if (!parent)
		return;

	RendererHandler     = parent;
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
    
	MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    attribVertex   = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
    attribColor    = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    glEnableVertexAttribArray(attribVertex);
    glEnableVertexAttribArray(attribColor);

    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::Render()
{
	static float k = 1.0;
    glEnable( GL_DEPTH_TEST );

    glUseProgram( program->ProgramID );
    TransformObj->TransformTranslate( -0.5, 0.0, -4.0);
    TransformObj->TransformRotate(k+=0.1, 1.0,  0.0, 0.0);
    TransformObj->TransformRotate(k+=0.1, 0.0,  1.0, 0.0);
    TransformObj->TransformRotate(k+=0.1, 0.0,  0.0, 1.0);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,(float*)TransformObj->TransformGetModelViewProjectionMatrix() );
    
    if ( useDrawElementAPI ){
        glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, cubeColors);
        glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, cubeVerts);
        glDrawElements(GL_LINE_LOOP, 36, GL_UNSIGNED_SHORT, cubeIndices);
    }
    else{
        glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, colorBufferData);
        glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, vertexBuffer);
        glDrawArrays(GL_LINE_LOOP, 0, 36);
    }
    GetScreenCoordinates();
}

void Cube::GetScreenCoordinates()
{
    // Get Screen Coordinate for cube vertices
    int	  viewport_matrix[4];
    glGetIntegerv( GL_VIEWPORT, viewport_matrix );
    screenCoordinateVector.clear();
    for(int i=0; i<sizeof(cubeVerts)/(sizeof(GLfloat)*3);i++){
        glm::vec3 obj(cubeVerts[i][0], cubeVerts[i][1], cubeVerts[i][2]);
        glm::vec4 viewport(viewport_matrix[0],viewport_matrix[1],viewport_matrix[2],viewport_matrix[3]);
        glm::vec3 screen = glm::project(obj, *TransformObj->TransformGetModelViewMatrix() , *TransformObj->TransformGetProjectionMatrix(), viewport);
        screenCoordinateVector.push_back(glm::vec2(screen.x,screen.y));
    }
}

void Cube::TouchEventDown( float x, float y )
{
    useDrawElementAPI = !useDrawElementAPI;
}
