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

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Cube::Cube( Renderer* parent )
{
	if (!parent)
		return;

	RenderObj			= parent;
	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= CubeType;
	LOGI("gl2: Cube constructor");
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
	PROGRAM *program                    = NULL;
	ProgramManager* ProgramManagerObj   = RenderObj->RendererProgramManager();
	Transform*	TransformObj            = RenderObj->RendererTransform();


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
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Cube::Render()
{
	LOGI("as: Render");
	Transform*	TransformObj            = RenderObj->RendererTransform();
	ProgramManager* ProgramManagerObj   = RenderObj->RendererProgramManager();
	PROGRAM* program                    = NULL;

	if (! ( program = ProgramManagerObj->Program( ( char * )"Cube" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"Cube" );
		ProgramManagerObj->AddProgram( program );
	}

        GLfloat  cubeVerts[][3] = {
            { -.10, -.10, -.10 },
            { -.10, -.10,  .10 },
            { -.10,  .10, -.10 },
            { -.10,  .10,  .10 },
            {  .10, -.10, -.10 },
            {  .10, -.10,  .10 },
            {  .10,  .10, -.10 },
            {  .10,  .10,  .10 },
        };

        GLfloat  cubeColors[][3] = {
            {  0.0,  0.0,  0.0 },
            {  0.0,  0.0,  1.0 },
            {  0.0,  1.0,  0.0 },
            {  0.0,  1.0,  1.0 },
            {  1.0,  0.0,  0.0 },
            {  1.0,  0.0,  1.0 },
            {  1.0,  1.0,  0.0 },
            {  1.0,  1.0,  1.0 },
        };

        //GLushort cubeIndices[] = {
        //    0, 1, 3, 2,
        //    4, 6, 7, 5,
        //    2, 3, 7, 6,
        //    0, 4, 5, 1,
        //    0, 2, 6, 4,
        //    1, 5, 7, 3
        //};
    GLushort cubeIndices[] = {0,1,2, 2,3,0,   // 36 of indices
                     0,3,4, 4,5,0,
                     0,5,6, 6,1,0,
                     1,6,7, 7,2,1,
                     7,4,3, 3,2,7,
                     4,7,6, 6,5,4};
	const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	char attribColor = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
	if (attribColor>=0){
        glEnableVertexAttribArray(attribColor);
        glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, cubeColors);
	}
	char attribVertex = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
	if (attribVertex>=0)
	{
        glEnableVertexAttribArray(attribVertex);
        glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, cubeVerts);
	}

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, cubeIndices);
    glDisableVertexAttribArray(attribVertex);
    glDisableVertexAttribArray(attribColor);
}