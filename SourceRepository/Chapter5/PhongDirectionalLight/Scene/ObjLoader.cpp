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
#define VERTEX_SHADER_PRG			( char * )"DirectionalLightVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"DirectionalLightFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/DirectionalLightVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/DirectionalLightFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define NORMAL_POSITION 1
#define TEX_COORD 2

GLuint OBJ_VAO_Id;
int stride;
GLvoid* offset;
GLvoid* offsetTexCoord;
char MVP;
char MV;
GLint NormalMatrix;
int ModelNumber = 0;
GLuint vertexBuffer;

// Namespace used
using std::ifstream;
using std::ostringstream;

// Model Name Array
#define STRING_LENGTH 100
char ModelNames[][STRING_LENGTH]= {"Sphere.obj", "Torus.obj", "Monkey.obj", "IsoSphere.obj", "Cone.obj","Plane.obj","SemiHollowCylinder.obj","CubeWithNormal.obj"};
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
ObjLoader::ObjLoader( Renderer* parent )
{
	if (!parent)
		return;

	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= ObjFileType;
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
    strcpy( fname, "/sdcard/Models/" );
#endif

    strcat( fname, ModelNames[ModelNumber]);
    
    objMeshModel    = waveFrontObjectModel.ParseObjModel(fname);
    IndexCount      = waveFrontObjectModel.IndexTotal();
    stride          = (2 * sizeof(vec3) )+ sizeof(vec2)+ sizeof(vec4);
    offset          = ( GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) );
    offsetTexCoord  = ( GLvoid*) ( sizeof(glm::vec3) );
    
    
    // Create the VBO for our obj model vertices.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, objMeshModel->vertices.size() * sizeof(objMeshModel->vertices[0]), &objMeshModel->vertices[0], GL_STATIC_DRAW);
    
    
    // Create the VAO, this will store the vertex attributes into vectore state.
    glGenVertexArrays(1, &OBJ_VAO_Id);
    glBindVertexArray(OBJ_VAO_Id);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
    glEnableVertexAttribArray(NORMAL_POSITION);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, stride, offsetTexCoord);
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
	if ( program = ProgramManagerObj->Program( ( char * )"DirectionalLight" ) )
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
	ProgramManager* ProgramManagerObj   = MapRenderHandler->RendererProgramManager();
	Transform*	TransformObj            = MapRenderHandler->RendererTransform();


	if (! ( program = ProgramManagerObj->Program( ( char * )"DirectionalLight" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"DirectionalLight" );
		ProgramManagerObj->AddProgram( program );
	}

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

    // Use Phong Shade Program
    glUseProgram( program->ProgramID );
    
    // Get Material property uniform variables
    char MaterialAmbient  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MaterialAmbient");
    char MaterialSpecular = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MaterialSpecular");
    char MaterialDiffuse  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MaterialDiffuse");
    char ShininessFactor  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"ShininessFactor");
    
    // Get Light property uniform variables
    char LightAmbient     = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"LightAmbient");
    char LightSpecular    = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"LightSpecular");
    char LightDiffuse     = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"LightDiffuse");
    char LightPosition     = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    
    if ( MaterialAmbient >= 0 ){
        glUniform3f(MaterialAmbient, 0.1f, 0.1f, 0.1f);
    }
    
    if ( MaterialSpecular >= 0){
        glUniform3f( MaterialSpecular, 1.0, 0.5, 0.5 );
    }
    
    if ( MaterialDiffuse >= 0 ){
        glm::vec3 color = glm::vec3(1.0, 0., 0.0);
        glUniform3f( MaterialDiffuse, color.r, color.g, color.b );
    }

    if ( LightAmbient >= 0 ){
        glUniform3f( LightAmbient, 1.0f, 1.0f, 1.0f );
    }
    
    if ( LightSpecular >=  0 ){
        glUniform3f( LightSpecular, 1.0, 1.0, 1.0 );
    }

    if ( LightDiffuse >= 0 ){
        glUniform3f(LightDiffuse, 1.0f, 1.0f, 1.0f);
    }

    if ( ShininessFactor >= 0 ){
        glUniform1f(ShininessFactor, 40);
    }

    if ( LightPosition >= 0 ){
        glm::vec3 lightPosition(0.0, 20.0, 10);
    }
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectionMatrix" );
    MV  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewMatrix" );
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
    // Use Specular program
    glUseProgram(program->ProgramID);
    
    char LightPosition     = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    float radius = 5.0;
    glm::vec3 lightPosition;
    if ( LightPosition >= 0 ){
        static double i = 0;
        i += 3.14 / 50;
        lightPosition = glm::vec3(cos(i) * radius, sin(i) * radius, 0.0);
        glUniform3fv(LightPosition, 1, (float*)&lightPosition);
    }
    
    // Apply Transformation.
	TransformObj->TransformPushMatrix();
    static float rot = 45.0;
	TransformObj->TransformRotate(rot , 1.0, 0.0, 1.0);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
    glUniformMatrix4fv( MV, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewMatrix() );
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
