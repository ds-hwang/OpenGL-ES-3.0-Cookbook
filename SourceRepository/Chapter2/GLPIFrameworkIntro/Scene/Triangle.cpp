#include "Triangle.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"BlueTriangleVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"BlueTriangleFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/BlueTriangleVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/BlueTriangleFragment.glsl"
#endif

// Namespace used
using std::ifstream;
using std::ostringstream;
GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
GLfloat gTriangleColors[]   = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

// Global Object Declaration

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Triangle::Triangle( Renderer* parent )
{
	if (!parent)
		return;

	RenderHandler       = parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= TriangleType;
    degree              = 0;
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
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Triangle::InitModel()
{
	if (!(program = ProgramManagerObj->Program( (char*) "Triangle"))){
		program = ProgramManagerObj->ProgramInit( (char *) "Triangle" );
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
	}
	m = freeCache( m );

	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
	}
	m = freeCache( m );

    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Triangle::Render()
{
    glUseProgram( program->ProgramID );

    radian = degree++/57.2957795;
    
    // Query and send the uniform variable.
    radianAngle          = glGetUniformLocation(program->ProgramID, "RadianAngle");
    glUniform1f(radianAngle, radian);

    
    positionAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
	colorAttribHandle    = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    
    glVertexAttribPointer(positionAttribHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    glVertexAttribPointer(colorAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, gTriangleColors);
    
    
    glEnableVertexAttribArray(positionAttribHandle);
    glEnableVertexAttribArray(colorAttribHandle);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}