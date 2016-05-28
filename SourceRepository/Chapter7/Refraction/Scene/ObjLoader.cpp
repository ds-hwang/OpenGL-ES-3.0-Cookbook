#include "ObjLoader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
//#import <fstream>
using namespace glm;

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"RefractionVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"RefractionFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/RefractionVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/RefractionFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define NORMAL_POSITION 1

// Model Name Array
#define STRING_LENGTH 100
char ModelNames[][STRING_LENGTH]= {"Plane.obj","Sphere1.obj", "Torus.obj", "Monkey.obj", "IsoSphere.obj"};
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
    ModelNumber         = 0;
    glEnable	( GL_DEPTH_TEST );
    
    LoadMesh();
}

void ObjLoader::ReleaseMeshResources()
{
    glDeleteVertexArrays(1, &OBJ_VAO_Id);
    glDeleteBuffers(1, &vertexBuffer);
}

void ObjLoader::SwitchMesh()
{
    // Release the old resources
    ReleaseMeshResources();
    
    // Get the new mesh model number
    ModelNumber++;
    ModelNumber %= sizeof(ModelNames)/(sizeof(char)*STRING_LENGTH);
    
    // Load the new mesh model
    LoadMesh();
}

void ObjLoader::LoadMesh()
{
    char fname[MAX_PATH]= {""};
#ifdef __IPHONE_4_0
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/GLPIFramework/Models/" );
#endif

    strcat( fname, ModelNames[ModelNumber]);
    
    objMeshModel    = waveFrontObjectModel.ParseObjModel(fname);
    IndexCount      = waveFrontObjectModel.IndexTotal();
    stride          = (2 * sizeof(vec3) ) + sizeof(vec2) + sizeof(vec4);
    offset          = ( GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) );
    
    // Create the VBO for our obj model vertices.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, objMeshModel->vertices.size() * sizeof(objMeshModel->vertices[0]), &objMeshModel->vertices[0], GL_STATIC_DRAW);
    
    
    // Create the VAO, this will store the vertex attributes into vector state.
    glGenVertexArrays(1, &OBJ_VAO_Id);
    glBindVertexArray(OBJ_VAO_Id);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(NORMAL_POSITION);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(NORMAL_POSITION, 3, GL_FLOAT, GL_FALSE, stride, offset);
    glBindVertexArray(0);
    
    objMeshModel->vertices.clear();
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

	if (! ( program = ProgramManagerObj->Program( ( char * )"ReflectionShader" ) ) ){
		program = ProgramManagerObj->ProgramInit( ( char * )"ReflectionShader" );
		ProgramManagerObj->AddProgram( program );
	}

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

    glUseProgram( program->ProgramID );
    char uniformTex = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "CubeMap" );
    if (uniformTex >= 0) {
        glUniform1i(uniformTex, 1);
    }
    char Camera = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"CameraPosition");
    if (Camera >= 0){
        glm::vec3 cp = RendererHandler->getCameraPosition();
        glUniform3fv(Camera, 1, (float*)&cp);
    }

    char indexOfRefract = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"RefractIndex");
    if (indexOfRefract >= 0){
        float iOfR = 0.85f;
        glUniform1f(indexOfRefract, iOfR);
    }
    

    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
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
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Use Specular program
    glUseProgram(program->ProgramID);
    glActiveTexture(GL_TEXTURE1);

    // Apply Transformation.
	TransformObj->TransformPushMatrix();
    static float rot = 0.0;
	TransformObj->TransformRotate(rot++ , 1.0, 1.0, 1.0);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    glUniformMatrix4fv( M, 1, GL_FALSE,( float * )TransformObj->TransformGetModelMatrix() );

    glm::mat4 matrix    = *(TransformObj->TransformGetModelViewMatrix());
    glm::mat3 normalMat = glm::mat3( glm::vec3(matrix[0]), glm::vec3(matrix[1]), glm::vec3(matrix[2]) );
    glUniformMatrix3fv( NormalMatrix, 1, GL_FALSE, (float*)&normalMat );
    TransformObj->TransformPopMatrix();
    
    // Bind with Vertex Array Object for OBJ
    glBindVertexArray(OBJ_VAO_Id);
    
    // Draw Geometry
    glDrawArrays(GL_TRIANGLES, 0, IndexCount );
    
    glBindVertexArray(0);
}

void ObjLoader::TouchEventDown( float x, float y )
{
    SwitchMesh();
}

void ObjLoader::TouchEventMove( float x, float y )
{
}

void ObjLoader::TouchEventRelease( float x, float y )
{
}
