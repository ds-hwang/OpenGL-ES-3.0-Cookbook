#include "SimpleTexture.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
#include "CustomScene.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"FXAAVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"FXAAFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/FXAAVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/FXAAFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

#define GetUniform(X,Y) ProgramManagerObj->ProgramGetUniformLocation(X,Y);


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
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void SimpleTexture::InitModel()
{
    program = ProgramManagerObj->ProgramLoad("square", VERTEX_SHADER_PRG, FRAGMENT_SHADER_PRG);

    glUseProgram( program->ProgramID );
    
    MVP                 = GetUniform( program, (char*) "ModelViewProjectMatrix" );
    TEX                 = GetUniform( program, (char*) "Tex1" );
    SCREEN_COORD_X      = GetUniform( program, (char*) "ScreenCoordX" );
    SCREEN_RESOLUTION   = GetUniform( program, (char*) "FBS");
    Scene* sceneRTS     = this->SceneHandler->getRenderer()->getScene((char*) "RenderToTexScene");
    CustomScene* cs     = dynamic_cast<CustomScene*>(sceneRTS);
    
    glm::vec2 resolution(cs->getFBO()->GetWidth(), cs->getFBO()->GetHeight());
    glUniform2fv(SCREEN_RESOLUTION, 1, &resolution[0]);
    

    glActiveTexture (GL_TEXTURE0);
    glUniform1i(TEX, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Model::InitModel();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

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
    glUniform1f(SCREEN_COORD_X, x);
    Model::TouchEventDown(x,y);
}

void SimpleTexture::TouchEventMove( float x, float y )
{
    glUniform1f(SCREEN_COORD_X, x);
    Model::TouchEventMove(x,y);
}

void SimpleTexture::TouchEventRelease( float x, float y )
{
    Model::TouchEventRelease(x,y);
}
