#include "ObjLoader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
#include "WaveFrontOBJ.h"
#import <fstream>
using namespace glm;

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"ReflectionVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"ReflectionFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/ReflectionVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/ReflectionFragment.glsl"
#endif

GLuint OBJ_VAO_Id;
char MVP;
char MV;
char M;
char NormalMatrix;
#define VERTEX_POSITION 0
#define NORMAL_POSITION 1
int stride;          
const GLvoid* offset;

// Namespace used
using std::ifstream;
using std::ostringstream;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
ObjLoader::ObjLoader( Renderer* parent )
{
	if (!parent)
		return;

	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= ObjFileType;
    char fname[MAX_PATH]= {""};
    cameraPosition      = vec3(0.0, 0.0, -15);
    glEnable	( GL_DEPTH_TEST );

	#ifdef __IPHONE_4_0
		GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
    #else
	    strcpy( fname, "/sdcard/Models/" );
    #endif

    //strcat( fname, "SemiHollowCylinder.obj" );
    strcat( fname, "Sphere1.obj" );
    //strcat( fname, "Plane.obj" );
    //strcat( fname, "Torus.obj" );
    //strcat( fname, "Plane.obj" );
    //strcat( fname, "IsoSphere.obj" );
    //strcat( fname, "Cylinder.obj" );

    objMeshModel = waveFrontObjectModel.ParseObjModel(fname);
    IndexCount = waveFrontObjectModel.IndexTotal();
    
    stride    = (2 * sizeof(vec3) )+ sizeof(vec2) + sizeof(vec4);
    offset    = (const GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) );

    // Create the VBO for our obj model vertices.
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, objMeshModel->vertices.size() * sizeof(objMeshModel->vertices[0]), &objMeshModel->vertices[0], GL_STATIC_DRAW);
    objMeshModel->vertices.clear();
    VertexBuffer    = vertexBuffer;
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
ObjLoader::~ObjLoader()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"ReflectionShader" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ObjLoader::InitModel()
{
	ProgramManager* ProgramManagerObj   = RendererHandler->RendererProgramManager();
	Transform*	TransformObj            = RendererHandler->RendererTransform();


	if (! ( program = ProgramManagerObj->Program( ( char * )"ReflectionShader" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"ReflectionShader" );
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
    
    char MaterialAmbient   = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"MaterialAmbient");
    char MaterialSpecular  = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"MaterialSpecular");
    char MaterialDiffuse   = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"MaterialDiffuse");
    char LightAmbient      = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"LightAmbient");
    char LightSpecular     = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"LightSpecular");
    char LightDiffuse      = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"LightDiffuse");
    char ShininessFactor   = ProgramManagerObj->ProgramGetUniformLocation(program,     (char*)"ShininessFactor");
    char LightPosition = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    char Camera = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"CameraPosition");
    glActiveTexture(GL_TEXTURE1);
    char uniformTex = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "CubeMap" );
    if (uniformTex >= 0) {
        glUniform1i(uniformTex, 1);
    }
    if (MaterialAmbient >= 0){
        glUniform3f(MaterialAmbient, 0.04f, 0.04f, 0.04f);
    }
    
    if (MaterialSpecular >= 0){
        glUniform3f(MaterialSpecular, 1.0, 0.5, 0.5);
    }
    
    glm::vec3 color = glm::vec3(1.0, 0.5, 0.0) * 0.75f;    
    if (MaterialDiffuse >= 0){
        glUniform3f(MaterialDiffuse, color.r, color.g, color.b);
    }

    if (LightAmbient >= 0){
        glUniform3f(LightAmbient, 1.0f, 1.0f, 1.0f);
    }
    
    if (LightSpecular >= 0){
        glUniform3f(LightSpecular, 1.0, 1.0, 1.0);
    }

    if (LightDiffuse >= 0){
        glUniform3f(LightDiffuse, 1.0f, 1.0f, 1.0f);
    }

    if (ShininessFactor >= 0){
        glUniform1f(ShininessFactor, 40);
    }

    if (LightPosition >= 0){
        glm::vec3 lightPosition(10.0, 10.0, 10);
        glUniform3fv(LightPosition, 1, (float*)&lightPosition);
    }

    if (Camera >= 0){
        glm::vec3 cp = RendererHandler->getCameraPosition();
        glUniform3fv(Camera, 1, (float*)&cp);
    }

    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    MV  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWMATRIX" );
    M   = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELMATRIX" );
    NormalMatrix  = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"NormalMatrix");
    
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ObjLoader::Render()
{
    glEnable(GL_CULL_FACE);

    glUseProgram(program->ProgramID);

	TransformObj->TransformPushMatrix();
    static float rot = 0.0;
	TransformObj->TransformRotate(rot= rot+5 , 1.0, 1.0, 1.0);
	TransformObj->TransformScale(2.0 , 2.0, 2.0);


    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    glUniformMatrix4fv( MV, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewMatrix() );
    glUniformMatrix4fv( M, 1, GL_FALSE,( float * )TransformObj->TransformGetModelMatrix() );
    glm::mat4 matrix    = *(TransformObj->TransformGetModelViewMatrix());
    glm::mat3 normalMat = glm::mat3( glm::vec3(matrix[0]), glm::vec3(matrix[1]), glm::vec3(matrix[2]) );
    glUniformMatrix3fv( NormalMatrix, 1, GL_FALSE, (float*)&normalMat );
	TransformObj->TransformPopMatrix();

    
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(NORMAL_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(NORMAL_POSITION, 3, GL_FLOAT, GL_FALSE, stride, offset);

    glDrawArrays(GL_TRIANGLES, 0, IndexCount );
    glDisableVertexAttribArray(VERTEX_POSITION);
    
}

void ObjLoader::TouchEventDown( float x, float y )
{
}

void ObjLoader::TouchEventMove( float x, float y )
{
}

void ObjLoader::TouchEventRelease( float x, float y )
{
}

