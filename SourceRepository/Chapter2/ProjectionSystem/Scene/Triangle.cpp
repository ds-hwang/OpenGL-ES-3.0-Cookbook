#include "Triangle.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"TriangleVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"TriangleFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/TriangleVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/TriangleFragment.glsl"
#endif


// Namespace used
using std::ifstream;
using std::ostringstream;

// Global Object Declaration
static float k=0;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Triangle::Triangle( Renderer* parent )
{
	if (!parent)
		return;

	RenderObj			= parent;
	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= TriangleType;
	LOGI("gl2: Triangle constructor");

	colors[0] = 0.0; colors[1] = 0.0; colors[2] = 1.0;
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 1.0;
	colors[6] = 0.0; colors[7] = 0.0; colors[8] = 1.0;
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
Triangle::~Triangle()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"Triangle" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void Triangle::InitModel()
{
	if (! ( program = ProgramManagerObj->Program( ( char * )"Triangle" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"Triangle" );
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
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void Triangle::Render()
{
    glUseProgram(program->ProgramID);
	Transform*	TransformObj            = RenderObj->RendererTransform();

	RenderTriangle();
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void Triangle::RenderTriangle()
{
	char uniform = ProgramManagerObj->ProgramGetUniformLocation( program, (char*)"MODELVIEWPROJECTIONMATRIX" );

	if ( uniform >= 0 )
	{
		glUniformMatrix4fv( uniform, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	}

	const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	char attrib = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
    glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    glEnableVertexAttribArray(attrib);

	char attribColor = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(attribColor);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attrib);
}

void Triangle::TouchEventDown( float x, float y )
{
	colors[0] = 1.0; colors[1] = 0.0; colors[2] = 0.0;
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 0.0;
	colors[6] = 1.0; colors[7] = 0.0; colors[8] = 0.0;
}

void Triangle::TouchEventMove( float x, float y )
{
	colors[0] = 0.0; colors[1] = 1.0; colors[2] = 0.0;
	colors[3] = 0.0; colors[4] = 1.0; colors[5] = 0.0;
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0;
	static float lastX=0;
	if (x-lastX>0)
		k+=2.50;
	else
		k-=2.50;
	lastX = x;
}

void Triangle::TouchEventRelease( float x, float y )
{
	colors[0] = 0.0; colors[1] = 0.0; colors[2] = 1.0;
	colors[3] = 0.0; colors[4] = 0.0; colors[5] = 1.0;
	colors[6] = 0.0; colors[7] = 0.0; colors[8] = 1.0;
}
