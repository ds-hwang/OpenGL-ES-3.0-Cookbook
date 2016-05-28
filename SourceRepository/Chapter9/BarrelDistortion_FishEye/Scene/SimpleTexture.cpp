//
//  SimpleTexture.cpp
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#include "SimpleTexture.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"Vertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"BarrelDistFishEyeFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/Vertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/BarrelDistFishEyeFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

#define GetUniform ProgramManagerObj->ProgramGetUniformLocation

static float lastX = 0;
static float lastY = 0;
#define PACE 1.0
static float deltaX = 0;
static float deltaY = 0;

float texCoords[8] ={   0.0f, 0.0f,
                        1.0f, 0.0f,
                        0.0f, 1.0f,
                        1.0f, 1.0f
                    };

float quad[12]     ={
                        -1.0f, -1.0f,  0.0f,
                         1.0f, -1.0f,  0.0f,
                        -1.0f, 1.0f, -0.0f,
                         1.0f, 1.0f, -0.0f
                    };

// Namespace used
using std::ifstream;
using std::ostringstream;

/*!
 Simple Default Constructor
 
 \param[in] None.
 \return None
 
 */
SimpleText::SimpleText( Renderer* parent )
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= CubeType;
    glEnable	( GL_DEPTH_TEST );
}


/*!
 Simple Destructor
 
 \param[in] None.
 \return None
 
 */
SimpleText::~SimpleText()
{
	PROGRAM* program = NULL;
	if ( (program = ProgramManagerObj->Program( ( char * )"square" )) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
 Initialize the scene, reserve shaders, compile and cache program
 
 \param[in] None.
 \return None
 
 */
void SimpleText::InitModel()
{
    
    // Vertical Blur Shader
    if (! ( program = ProgramManagerObj->Program( ( char * )"BDFishEye" ) ) )
    {
        program = ProgramManagerObj->ProgramInit( ( char * )"BDFishEye" );
        ProgramManagerObj->AddProgram( program );
    }
    
    program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
    program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);
    
    /////////// Simple Vertex & vertical Blur Fragment shader //////////
    CACHE* m = reserveCache( VERTEX_SHADER_PRG, true );
    
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

    MVP = GetUniform( program,(char*)"MODELVIEWPROJECTIONMATRIX" );
    TEX = GetUniform( program, (char *) "Tex1" );
    SCREEN_COORD_X = GetUniform( program, (char *) "ScreenCoordX" );
    BARREL_POWER = GetUniform( program, (char *) "BarrelPower" );
    glUniform1f(BARREL_POWER, 1.0);

    return;
}

void SimpleText::Render()
{
    program = ProgramManagerObj->Program( ( char * )"BDFishEye" );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(program->ProgramID);
    
    TransformObj->TransformPushMatrix();
    
    glUniform1i(TEX, 0);
    
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
    glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, quad);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    TransformObj->TransformPopMatrix();
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
}

void SimpleText::TouchEventDown( float x, float y )
{
    lastX = x;
    lastY = y;
}

void SimpleText::TouchEventMove( float x, float y )
{
    //    rotationX += (x - lastX) * PACE;
    //    rotationY += (y - lastY) * PACE;
    glUseProgram(program->ProgramID);
    //glUniform1f(SCREEN_COORD_X, x);
    static float barrelPower = 1.0;
    float width = RendererHandler->screenWidthPixel();
    if(x>lastX)
        barrelPower += 0.04;
    else
        barrelPower -= 0.04;
    if(y<lastY)
        barrelPower += 0.04;
    else
        barrelPower -= 0.04;
    
    if(barrelPower <= 0.5 ){
        barrelPower = 0.5;
    }
    
    glUniform1f(BARREL_POWER, barrelPower);
    printf("%f,",barrelPower);
    lastX = x;
    lastY = y;
}

void SimpleText::TouchEventRelease( float x, float y )
{
    deltaX = x - lastX;
    deltaY = y - lastY;
}

