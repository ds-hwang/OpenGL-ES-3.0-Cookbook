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
#define VERTEX_SHADER_PRG			( char * )"FBOVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"FBOFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/FBOVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/FBOFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

static float rotationX = 0.0;
static float rotationY = 0.0;
static float lastX = 0;
static float lastY = 0;
#define PACE 1.0
static float deltaX = 0;
static float deltaY = 0;
static float DecelerationFactor = 0.5;

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
SimpleTexture::SimpleTexture( Renderer* parent )
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= CubeType;
    char fname[MAX_PATH]= {""};
    glEnable	( GL_DEPTH_TEST );
}


/*!
 Simple Destructor
 
 \param[in] None.
 \return None
 
 */
SimpleTexture::~SimpleTexture()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"square" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
 Initialize the scene, reserve shaders, compile and cache program
 
 \param[in] None.
 \return None
 
 */
void SimpleTexture::InitModel()
{
	ProgramManager* ProgramManagerObj   = RendererHandler->RendererProgramManager();
	Transform*	TransformObj            = RendererHandler->RendererTransform();
    
    
	if (! ( program = ProgramManagerObj->Program( ( char * )"square" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"square" );
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
        freeCache(m);
	}
    
	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        freeCache(m);
	}
    
    
    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );
    
    glUseProgram( program->ProgramID );
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    TEX = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "Tex1" );
    
    return;
}

/*!
 Initialize the scene, reserve shaders, compile and cache program
 
 \param[in] None.
 \return None
 
 */
void SimpleTexture::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_CULL_FACE);
	glUseProgram(program->ProgramID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
	TransformObj->TransformPushMatrix();

	TransformObj->TransformRotate(rotationX, 0.0, 1.0, 0.0);
	TransformObj->TransformRotate(rotationY, 1.0, 0.0, 0.0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glUniform1i(TEX, 0);

    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
	glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, quad);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	TransformObj->TransformPopMatrix();
    
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glDisableVertexAttribArray(VERTEX_POSITION);
	glDisableVertexAttribArray(TEX_COORD);
    if ( deltaX != 0.0 ){
        if( deltaX > 0.0 ){
            deltaX -= DecelerationFactor;
        }
        else{
            deltaX += DecelerationFactor;
        }
        rotationX += deltaX;
    }

    if ( deltaY != 0.0 ){
        if( deltaY > 0.0 ){
            deltaY -= DecelerationFactor;
        }
        else{
            deltaY += DecelerationFactor;
        }
        rotationY += deltaY;
    }
}

void SimpleTexture::TouchEventDown( float x, float y )
{
    lastX = x;
    lastY = y;
}

void SimpleTexture::TouchEventMove( float x, float y )
{
    rotationX += (x - lastX) * PACE;
    rotationY += (y - lastY) * PACE;
    lastX = x;
    lastY = y;
}

void SimpleTexture::TouchEventRelease( float x, float y )
{
    deltaX = x - lastX;
    deltaY = y - lastY;
}



