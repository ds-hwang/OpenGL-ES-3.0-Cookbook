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
#define VERTEX_SHADER_PRG			( char * )"BumpVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"BumpFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/BumpVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/BumpFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define NORMAL_POSITION 1
#define TEX_COORD 2
#define TANGENTS 3

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

static float rotationX = 0.0;
static float rotationY = 0.0;
static float lastX = 0;
static float lastY = 0;
#define PACE 1.0
static float deltaX = 0;
static float deltaY = 0;
static float DecelerationFactor = 0.5;

// Model Name Array
#define STRING_LENGTH 100
char ModelNames[][STRING_LENGTH]= {"Sphere.obj"};
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
    //strcat( fname, "ogre_diffuse.png" );
    strcat( fname, "earthcolor.png" );
    //strcat( fname, "cartoon.png" );
    //strcat( fname, "grass.png" );
    image = new PngImage();
    image->loadImage(fname);

    memset(fname, 0, MAX_PATH);
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/Images/" );
#endif
    //strcat( fname, "ogre_normalmap.png" );
    strcat( fname, "earthnormal.png" );
    imageNormal = new PngImage();
    imageNormal->loadImage(fname);

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
    glEnableVertexAttribArray(TANGENTS);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, stride, offsetTexCoord);
    glVertexAttribPointer(NORMAL_POSITION, 3, GL_FLOAT, GL_FALSE, stride, offsetNormal);
    glVertexAttribPointer(TANGENTS, 4, GL_FLOAT, GL_FALSE, stride, offsetTangent);
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
    
    char MaterialAmbient = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"MaterialAmbient");
    char MaterialSpecular= ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"MaterialSpecular");
    char MaterialDiffuse = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"MaterialDiffuse");
    char LightAmbient    = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightAmbient");
    char LightSpecular   = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightSpecular");
    char LightDiffuse    = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightDiffuse");
    char ShininessFactor = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"ShininessFactor");
    char LightPosition   = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    char BackLightDiffuse= ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"BackFaceLightDiffuse");
    
    if (MaterialAmbient >= 0){
        glUniform3f(MaterialAmbient, 0.10f, 0.10f, 0.10f);
    }
    
    if (MaterialSpecular >= 0){
        glUniform3f(MaterialSpecular, .3, .3, .3);
    }
    
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
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
    if (BackLightDiffuse >= 0){
        glUniform3f(BackLightDiffuse, 1.0f, 1.0f, 1.0f);
    }

    if (ShininessFactor >= 0){
        glUniform1f(ShininessFactor, 2.0);
    }
    
    if ( LightPosition >= 0 ){
        glm::vec3 lightPosition(0.0, 10.0, 10.0);
        glUniform3fv(LightPosition, 1, (float*)&lightPosition);
    }
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectionMatrix" );
    MV  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewMatrix" );
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

    TEX_NORMAL = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ImageTextureNormal" );
    
    glActiveTexture (GL_TEXTURE1);
	if (imageNormal) {
        glBindTexture(GL_TEXTURE_2D, imageNormal->getTextureID());
        
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
    glUniform1i(TEX_NORMAL, 1);

    // Apply Transformation.
	TransformObj->TransformPushMatrix();
    static float rot = 0.0;
	TransformObj->TransformRotate(rot++ , 0.0, 1.0, 0.0);
	TransformObj->TransformRotate(180.0 , 0.0, 0.0, 1.0);
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
    //SwitchMesh();
    lastX = x;
    lastY = y;
}

void ObjLoader::TouchEventMove( float x, float y )
{
    rotationX += (x - lastX) * PACE;
    rotationY += (y - lastY) * PACE;
    lastX = x;
    lastY = y;
}

void ObjLoader::TouchEventRelease( float x, float y )
{
    deltaX = x - lastX;
    deltaY = y - lastY;
}

