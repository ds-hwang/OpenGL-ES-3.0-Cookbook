#include "Button.h"

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
Button::Button( Scene* parent, Model* model, ModelType type, glm::vec3* vtx, glm::vec2* textureCoordinates, char* texture ):Model(parent, model, type)
{
	if (!parent)
		return;
    
	SceneHandler        = parent;
	modelType 			= ImageDemoType;
    clicked             = false;
    isPicked            = false;
    
    char fname[MAX_PATH]= {""};
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/GLPIFramework/Images/" );
#endif
    
    strcat( fname, texture );
    image = new PngImage();
    image->loadImage(fname);

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
        float w = (float)image->imageWidth();
        float h = (float)image->imageWidth();
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
Button::~Button()
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
void Button::InitModel()
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
            m = freeCache( m );
        }
        
        /////////// Fragment shader //////////////////////////
        m = reserveCache( FRAGMENT_SHADER_PRG, true );
        if( m ) {
            if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
            m = freeCache( m );
        }
        
        
        if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );
	}
    
    glUseProgram( program->ProgramID );
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectMatrix" );
    TEX = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "Tex1" );
    
    // Get Material property uniform variables
    Picking  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"Picked");

    
    glActiveTexture (GL_TEXTURE0);
    glUniform1i(TEX, 0);
	if (image) {
        //glBindTexture(GL_TEXTURE_2D, image->getTextureID());
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    Model::InitModel();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Button::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(program->ProgramID);
    
    // Disable culling
    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture (GL_TEXTURE0);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glUniform1i(TEX, 0);
    if (image) {
        //glBindTexture(GL_TEXTURE_2D, image->getTextureID());
    }

    if(isPicked){
        if ( Picking >= 0 ){
            glUniform1i(Picking, 1);
        }
    }
    else{
        if ( Picking >= 0 ){
            glUniform1i(Picking, 0);
        }
    }

    
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
    
    if(clicked){
        Scene*  MeshScene = SceneHandler->getRenderer()->getScene((char*)"MeshScene");
        if(!MeshScene){
            return;
        }
        Camera* camera = MeshScene->getCamera("Camera1");
        
        if(name.compare("Direction Up") == 0){
            camera->Rotate(glm::vec3(.0, 0.0, 1.0), 4.0);
        }
        
        if (name.compare("Direction Down") == 0){
            camera->Rotate(glm::vec3(.0, 0.0, 1.0), -4.0);
        }
        
        if (name.compare("Direction Left") == 0){
            camera->Rotate(glm::vec3(0.0, 1.0, 0.0), 4.0);
        }
        
        if (name.compare("Direction Right") == 0){
            camera->Rotate(glm::vec3(0.0, 1.0, 0.0), -4.0);
        }
        
        if (name.compare("Direction Forward") == 0){
            camera->MoveForwards(5);
        }
        
        if (name.compare("Direction Backward") == 0){
            camera->MoveBackwards(5);
        }
    }
}

void Button::TouchEventDown( float x, float y )
{
    GLint viewport_matrix[4]	= { 0, 0, 0, 0 };
    glGetIntegerv( GL_VIEWPORT, viewport_matrix );
    glm::vec4 viewport(viewport_matrix[0],viewport_matrix[1],viewport_matrix[2],viewport_matrix[3]);
    glm::vec3 win(x, viewport_matrix[3]-y, 0.0);
    glm::mat4 a, b;
    //glm::unProject(win, a, b, viewport);
    glm::vec3 nearPoint = glm::unProject(win, *TransformObj->TransformGetModelViewMatrix(), *TransformObj->TransformGetProjectionMatrix(), viewport);
    win.z = 1.0;
    glm::vec3 farPoint = glm::unProject(win, *TransformObj->TransformGetModelViewMatrix(), *TransformObj->TransformGetProjectionMatrix(), viewport);
    Ray ray0(nearPoint, farPoint-nearPoint);
    
    glm::vec3 intersectionPoint;
    
    if(IntersectWithRay( ray0, intersectionPoint)){
        printf("Intersect with %s", this->GetName().c_str());
        isPicked = !isPicked;
        clicked = true;
        return;
    }
    
    Model::TouchEventDown(x,y);
}

void Button::TouchEventMove( float x, float y )
{
    Model::TouchEventMove(x,y);
}

void Button::TouchEventRelease( float x, float y )
{
    clicked = false;
    isPicked = false;
    Model::TouchEventRelease(x,y);
}

// Discuss in There More section.

bool Button::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
{
    int index0, index1, index2;
    
    // CHECK FIRST TRIANGLE
    vec4 p0 = vec4(vertices[0], 1.0);
    vec4 p1 = vec4(vertices[1], 1.0);
    vec4 p2 = vec4(vertices[2], 1.0);
    
    p0 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p0;
    p1 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p1;
    p2 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p2;
    if ( glm::intersectLineTriangle(ray0.destination, ray0.direction, vec3(p0.x,p0.y,p0.z), vec3(p1.x,p1.y,p1.z), vec3(p2.x,p2.y,p2.z), intersectionPoint)){
        return true;
    }

    // CHECK SECOND TRIANGLE
    p0 = vec4(vertices[1], 1.0);
    p1 = vec4(vertices[3], 1.0);
    p2 = vec4(vertices[2], 1.0);
    
    p0 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p0;
    p1 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p1;
    p2 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p2;
    if ( glm::intersectLineTriangle(ray0.destination, ray0.direction, vec3(p0.x,p0.y,p0.z), vec3(p1.x,p1.y,p1.z), vec3(p2.x,p2.y,p2.z), intersectionPoint)){
        return true;
    }

    
    return false;
}

