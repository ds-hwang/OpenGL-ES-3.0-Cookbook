#include "TextureQuadSphere.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"TexQuadSphereVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"TexQuadSphereFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/TexQuadSphereVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/TexQuadSphereFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1
#define VERTEX_COLOR 2

// Namespace used
using std::ifstream;
using std::ostringstream;

GLfloat vertexColors[12] ={
    0,0,0,
    1,0,0,
    1,1,0,
    0,1,0
};


float texCoords[8] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

float quad[8]     ={
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
};

// Global Object Declaration

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
TextureQuadSphere::TextureQuadSphere( Renderer* parent )
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
TextureQuadSphere::TextureQuadSphere()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"SphereTexQuad" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void TextureQuadSphere::InitModel()
{
	if (!(program = ProgramManagerObj->Program( (char*) "SphereTexQuad"))){
		program = ProgramManagerObj->ProgramInit( (char *) "SphereTexQuad" );
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
	

    //////////////////////////////////////////////////////
    /////////// Fragment shader //////////////////////////
    //////////////////////////////////////////////////////
	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        freeCache( m );
    }

    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
    SCREEN_COORD_X = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ScreenCoordX");
    SCREEN_COORD_Y = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ScreenCoordY");
    SCREEN_W = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ScreenWidth");
    SCREEN_H = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ScreenHeight");

    glUniform1f(SCREEN_W, 400.0f);
    glUniform1f(SCREEN_H, 400.0f);
    glUniform1f(SCREEN_COORD_X, 400.0f);
    glUniform1f(SCREEN_COORD_Y, 400.0f);

    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program
 
	\param[in] None.
	\return None
 
 */
void TextureQuadSphere::Render()
{
    glUseProgram( program->ProgramID );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    static float rot = 0.0;
    int items = 5;
    for(float i=-items;i<items+1;i++){
        for(float j=-items;j<items+1;j++){
            TransformObj->TransformPushMatrix();
            //TransformObj->TransformRotate(rot+=0.1, 1.0, 1.0, 1.0);
            TransformObj->TransformTranslate(i, j, 0.0);
            TransformObj->TransformScale(0.5, 0.5, 0.5);
            RenderSphere();
            TransformObj->TransformPopMatrix();
        }
    }
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void TextureQuadSphere::RenderSphere()
{
    positionAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
	colorAttribHandle    = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");
	GLuint w = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"ScreenWidth");
	GLuint h = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"ScreenHeight");
    float wd,hd;
    glUniform1f(w, wd = RenderHandler->screenWidthPixel());
    glUniform1f(h, hd = RenderHandler->screenHeightPixel());
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
    glEnableVertexAttribArray(VERTEX_COLOR);
    glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glVertexAttribPointer(VERTEX_POSITION, 2, GL_FLOAT, GL_FALSE, 0, quad);
    glVertexAttribPointer(VERTEX_COLOR, 3, GL_FLOAT, GL_FALSE, 0, vertexColors);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    
    // Draw triangle
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void TextureQuadSphere::TouchEventDown( float x, float y )
{
    glUseProgram(program->ProgramID);

    glUniform1f(SCREEN_COORD_X, x);
    glUniform1f(SCREEN_COORD_Y, RenderHandler->screenHeightPixel()-y);
}

void TextureQuadSphere::TouchEventMove( float x, float y )
{
    glUseProgram(program->ProgramID);
    glUniform1f(SCREEN_COORD_X, x);
    glUniform1f(SCREEN_COORD_Y, RenderHandler->screenHeightPixel()-y);
}

void TextureQuadSphere::TouchEventRelease( float x, float y )
{
}
