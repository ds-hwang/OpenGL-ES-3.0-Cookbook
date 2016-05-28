#include "SimpleTexture.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"SimpleTextureVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"SimpleTextureFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/SimpleTextureVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/SimpleTextureFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

// Namespace used
using std::ifstream;
using std::ostringstream;
using namespace glm;
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
SimpleTexture::SimpleTexture( Scene* parent, Model* model, ModelType type, glm::vec3* vtx, glm::vec2* textureCoordinates):Model(parent, model, type)
{
	if (!parent)
		return;
    
	SceneHandler        = parent;
	modelType 			= ImageDemoType;

    if(textureCoordinates){
        memcpy(texCoordinates, textureCoordinates, sizeof(glm::vec2)*4);
    }
    else{
        glm::vec2 texCoords[4] = {
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
        };
        memcpy(texCoordinates, texCoords, sizeof(glm::vec2)*4);
    }
    
    if(vtx){
        memcpy(vertices, vtx, sizeof(glm::vec3)*4);
    }
    else{
        int w = 64;
        int h = 64;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &w);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &h);
        glm::vec3 tempVtx[4] = {
            glm::vec3( 0.0f, 0.0f, 0.0f),
            glm::vec3( w, 0.0f, 0.0f),
            glm::vec3( 0.0f, h, 0.0f),
            glm::vec3( w,  h, 0.0f)
        };
        memcpy(vertices, tempVtx, sizeof(glm::vec3)*4);
    }    
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

	if (! ( program = ProgramManagerObj->Program( ( char * )"square" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"square" );
		ProgramManagerObj->AddProgram( program );
        program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
        program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);
        
        /////////// Vertex shader //////////////////////////
        CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );
        
        if( m ) {
            if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
            freeCache( m );
        }
        
        /////////// Fragment shader //////////////////////////
        m = reserveCache( FRAGMENT_SHADER_PRG, true );
        if( m ) {
            if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
            freeCache( m );
        }
        
        
        if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );
	}
    
    glUseProgram( program->ProgramID );
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectMatrix" );
    TEX = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "Tex1" );
    
    // Get Material property uniform variables
    //Picking  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"Picked");

    
    glActiveTexture (GL_TEXTURE0);
    glUniform1i(TEX, 0);
	//if (image) {
        //glBindTexture(GL_TEXTURE_2D, image->getTextureID());
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //}
    Model::InitModel();
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
    glUseProgram(program->ProgramID);
    
    // Disable culling
    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture (GL_TEXTURE0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1i(TEX, 0);

    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible)
    {
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
        
        glEnableVertexAttribArray(VERTEX_POSITION);
        glEnableVertexAttribArray(TEX_COORD);
        glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, &texCoordinates[0]);
        glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
        glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
        
        // Draw triangle
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void SimpleTexture::TouchEventDown( float x, float y )
{
    Model::TouchEventDown(x,y);
}

void SimpleTexture::TouchEventMove( float x, float y )
{
    Model::TouchEventMove(x,y);
}

void SimpleTexture::TouchEventRelease( float x, float y )
{
    Model::TouchEventRelease(x,y);
}
