#include "ObjLoader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
//#import <fstream>
using namespace glm;

#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"DiscardFragTexVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"DiscardFragTexFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/DiscardFragTexVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/DiscardFragTexFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define NORMAL_POSITION 1
#define TEX_COORD 2

GLuint OBJ_VAO_Id;
int stride;
GLvoid* offsetNormal;
GLvoid* offsetTexCoord;
GLvoid* offsetTangent;
char MVP;
char MV;
char TEX;
char TEX_NORMAL;

GLint NormalMatrix;
int ModelNumber = 0;
GLuint vertexBuffer;

// Namespace used
using std::ifstream;
using std::ostringstream;

// Model Name Array
#define STRING_LENGTH 100
char ModelNames[][STRING_LENGTH]= {"Sphere1.obj","CubeWithNormal.obj", "Plane.obj"};
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
    glEnable	( GL_DEPTH_TEST );
    char fname[MAX_PATH]= {""};
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/Images/" );
#endif
    strcat( fname, "stripPattern.png" );
    //strcat( fname, "contour.png" );
    //strcat( fname, "cp.png" );
    //strcat( fname, "Nuclear-explosion.png" );
    //strcat( fname, "waves.png" );
    image = new PngImage();
    image->loadImage(fname);

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
    stride          = (2 * sizeof(vec3) )+ sizeof(vec2) + sizeof(vec4);
    offsetNormal          = ( GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) );
    offsetTexCoord  = ( GLvoid*) ( sizeof(glm::vec3) );
    offsetTangent   = ( GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) + sizeof(vec3) );
    
    
    // Create the VBO for our obj model vertices.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, objMeshModel->vertices.size() * sizeof(objMeshModel->vertices[0]), &objMeshModel->vertices[0], GL_STATIC_DRAW);
    
    
    // Create the VAO, this will store the vertex attributes into vector state.
    glGenVertexArrays(1, &OBJ_VAO_Id);
    glBindVertexArray(OBJ_VAO_Id);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
    glEnableVertexAttribArray(NORMAL_POSITION);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, stride, offsetTexCoord);
    glVertexAttribPointer(NORMAL_POSITION, 3, GL_FLOAT, GL_FALSE, stride, offsetNormal);
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
	if ( program = ProgramManagerObj->Program( ( char * )"Grid" ) )
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


	if (! ( program = ProgramManagerObj->Program( ( char * )"Grid" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"Grid" );
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
    
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    MV  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWMATRIX" );
    NormalMatrix  = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"NormalMatrix");

    TEX = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ImageTexture" );
    
    glActiveTexture (GL_TEXTURE0);
	if (image) {
        glBindTexture(GL_TEXTURE_2D, image->getTextureID());
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ObjLoader::Render()
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Use Specular program
    glUseProgram(program->ProgramID);
    glUniform1i(TEX, 0);
    char LightPosition   = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    static float light = 0.0;
    if (LightPosition >= 0){
        glUniform3f(LightPosition, 0.0, 0.0, 100.0 );
    }

    // Apply Transformation.
	TransformObj->TransformPushMatrix();
    static float rot = 0.0;
	TransformObj->TransformRotate(rot++ , 1.0, 1.0, 1.0);
//	TransformObj->TransformRotate(rotationX, 0.0, 1.0, 0.0);
//	TransformObj->TransformRotate(rotationY, 1.0, 0.0, 0.0);
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

