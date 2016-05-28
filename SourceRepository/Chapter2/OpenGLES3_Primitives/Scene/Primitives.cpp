#include "Primitives.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"PrimitiveVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"PrimitiveFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/PrimitiveVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/PrimitiveFragment.glsl"
#endif


// Namespace used
using std::ifstream;
using std::ostringstream;

//By defaut orientation in anti clock wise direction.
const GLfloat vertices[] = {
     1.0f,  2.0f,
    -1.0f,  2.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f,
     1.0f,  0.0f,
    -1.0f,  0.0f,
     1.0f, -1.0f,
    -1.0f, -1.0f,
     1.0f, -2.0f,
    -1.0f, -2.0f
};

GLfloat  colors[] ={
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0
};



/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Primitives::Primitives( Renderer* parent )
{
	if (!parent)
		return;

	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= TriangleType;
    primitive           = 1;

}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
Primitives::Primitives()
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
void Primitives::InitModel()
{
	if (!(program = ProgramManagerObj->Program( ( char * )"Triangle" ))){
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
	mvp          = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MODELVIEWPROJECTIONMATRIX");
	attribVertex = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
	attribColor  = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
    
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Primitives::Render()
{
    glUseProgram(program->ProgramID);
    
	TransformObj->TransformPushMatrix();
	TransformObj->TransformTranslate(-0.0,  0.0, -4.0);
	RenderPrimitives();
	TransformObj->TransformPopMatrix();
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Primitives::RenderPrimitives()
{
    glDisable(GL_CULL_FACE);
    glLineWidth(10.0f);

    glUniformMatrix4fv( mvp, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
 
    glVertexAttribPointer(attribVertex, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(attribVertex);
    glEnableVertexAttribArray(attribColor);

    int primitiveArraySize = sizeof(vertices);
    int vertexSize         = (sizeof(float)*2);
    int numberOfElement    = primitiveArraySize/vertexSize;
    switch(primitive)
    {
        case GL_POINTS:
            glDrawArrays(primitive, 0, numberOfElement);
            break;

        case GL_LINES:
            glDrawArrays(primitive, 0, numberOfElement);
            break;
        case GL_LINE_LOOP:
            glDrawArrays(primitive, 0, numberOfElement);
            break;

        case GL_LINE_STRIP:
            glDrawArrays(primitive, 0, numberOfElement);
            break;
        case GL_TRIANGLES:
            glDrawArrays(primitive, 0, numberOfElement);
            break;

        case GL_TRIANGLE_STRIP:
            glDrawArrays(primitive, 0, numberOfElement);
            break;

        case GL_TRIANGLE_FAN:
            glDrawArrays(primitive, 0, numberOfElement);
            break;
    }
}

void Primitives::TouchEventDown( float x, float y )
{
    primitive++;
    primitive = primitive%7;

}

void Primitives::TouchEventMove( float x, float y )
{
}

void Primitives::TouchEventRelease( float x, float y )
{
}
