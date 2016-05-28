#include "Circle.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"AACircleVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"AACircleFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/AACircleVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/AACircleFragment.glsl"
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
Circle::Circle( Scene* parent, Model* model, ModelType type):Model(parent, model, type)
{
	if (!parent)
		return;
    
	SceneHandler        = parent;
	modelType 			= ImageDemoType;

    glm::vec2 texCoords[4] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
    };
    memcpy(texCoordinates, texCoords, sizeof(glm::vec2)*4);

    glm::vec3 tempVtx[4] = {
        glm::vec3( -0.5f, -0.5f, 0.0f),
        glm::vec3( -0.5f,  0.5f, 0.0f),
        glm::vec3(  0.5f, -0.5f, 0.0f),
        glm::vec3(  0.5f,  0.5f, 0.0f)
    };
    memcpy(vertices, tempVtx, sizeof(glm::vec3)*4);
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
Circle::~Circle()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"quad" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Circle::InitModel()
{
    program = ProgramManagerObj->ProgramLoad("quad", VERTEX_SHADER_PRG, FRAGMENT_SHADER_PRG);

    glUseProgram( program->ProgramID );
    
    MVP                 = GetUniform( program, (char*) "ModelViewProjectMatrix" );
    SCREEN_COORD_X      = GetUniform( program, (char*) "ScreenCoordX" );
    INNER_RADIUS        = GetUniform( program, (char*) "InnerRadius" );
    OUTER_RADIUS        = GetUniform( program, (char*) "OuterRadius" );
    PAINT_COLOR         = GetUniform( program, (char*) "PaintColor" );
    glUniform1f(INNER_RADIUS, 0.48);
    glUniform1f(OUTER_RADIUS, 0.50);
    glUniform3f(PAINT_COLOR, 1.0, 1.0, 0.0);
    Model::InitModel();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Circle::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(program->ProgramID);
    
    // Disable culling
    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

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

void Circle::TouchEventDown( float x, float y )
{
    glUniform1f(SCREEN_COORD_X, x);
    Model::TouchEventDown(x,y);
}

void Circle::TouchEventMove( float x, float y )
{
    glUniform1f(SCREEN_COORD_X, x);
    Model::TouchEventMove(x,y);
}

void Circle::TouchEventRelease( float x, float y )
{
    Model::TouchEventRelease(x,y);
}
