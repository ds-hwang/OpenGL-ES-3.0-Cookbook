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
#define VERTEX_SHADER_PRG			( char * )"GouraudVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"GouraudFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/GouraudVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/GouraudFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define NORMAL_POSITION 1
#define TEX_COORD 2

// Namespace used
using std::ifstream;
using std::ostringstream;

// Model Name Array
#define STRING_LENGTH 100
char ModelNames[][STRING_LENGTH]= {"SemiHollowCylinder.obj"/*0*/,"Sphere.obj"/*1*/, "Torus.obj"/*2*/, "Monkey.obj"/*3*/, "IsoSphere.obj"/*4*/, "Cone.obj"/*5*/,"CubeWithNormal.obj"/*6*/};
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
ObjLoader::ObjLoader( Scene* parent, Model* model, MeshType mesh, ModelType type ):Model(parent, model, type)
{
	if (!parent)
		return;

    glEnable	( GL_DEPTH_TEST );
    ModelNumber = (int)mesh;
    ModelNumber %= sizeof(ModelNames)/(sizeof(char)*STRING_LENGTH);
    transformation[0][0] = transformation[1][1] = transformation[2][2] = transformation[3][3] = 1.0;
    isPicked = false;
    LoadMesh();
}

void ObjLoader::ReleaseMeshResources()
{
    glDeleteVertexArrays(1, &OBJ_VAO_Id);
    glDeleteBuffers(1, &vertexBuffer);
}

void ObjLoader::SwitchMesh()
{
    return; //Switching Disabled.
    
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
    
    
    // Create the VAO, this will store the vertex attributes into vector state.
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
    
    //objMeshModel->vertices.clear();
}
/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
ObjLoader::~ObjLoader()
{
	PROGRAM* program = NULL;
	if ( program = (ProgramManagerObj->Program( ( char * )"GouraudShader" )) )
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
    Model::InitModel();

	if (! ( program = ProgramManagerObj->Program( ( char * )"GouraudShader" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"GouraudShader" );
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


    // Use Phong Shade Program
    glUseProgram( program->ProgramID );
    
    // Get Material property uniform variables
    MaterialAmbient  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MaterialAmbient");
    MaterialSpecular = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MaterialSpecular");
    MaterialDiffuse  = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"MaterialDiffuse");
    MaterialShininess= ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"ShininessFactor");
    
    // Get Light property uniform variables
    LightAmbient     = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"LightAmbient");
    LightSpecular    = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"LightSpecular");
    LightDiffuse     = ProgramManagerObj->ProgramGetUniformLocation(program,(char*)"LightDiffuse");
    LightPosition     = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    
    Picking = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"Picked");
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
    glUseProgram(program->ProgramID);
    ApplyMaterial();
    ApplyLight();
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

    if(isVisible){
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
        
        // Apply Transformation.
        glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
        glUniformMatrix4fv( MV, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewMatrix() );
        glm::mat4 matrix    = *(TransformObj->TransformGetModelViewMatrix());
        glm::mat3 normalMat = glm::mat3( glm::vec3(matrix[0]), glm::vec3(matrix[1]), glm::vec3(matrix[2]) );
        glUniformMatrix3fv( NormalMatrix, 1, GL_FALSE, (float*)&normalMat );
        
        // Bind with Vertex Array Object for OBJ
        glBindVertexArray(OBJ_VAO_Id);
        
        // Draw Geometry
        glDrawArrays(GL_TRIANGLES, 0, IndexCount );
        glBindVertexArray(0);
        TransformObj->TransformPopMatrix(); // Local Level
    }
    
    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

// Apply scenes light on the object
void ObjLoader::ApplyLight()
{
    for(int i =0; i<SceneHandler->getLights().size(); i++){
        Light*& light = SceneHandler->getLights().at(i);
        if ( LightAmbient >= 0 ){
            glUniform3f( LightAmbient, light->material.ambient.r, light->material.ambient.g, light->material.ambient.b );
        }
        
        if ( LightSpecular >=  0 ){
            glUniform3f( LightSpecular, light->material.specular.r, light->material.specular.g, light->material.specular.b );
        }
        
        if ( LightDiffuse >= 0 ){
            glUniform3f(LightDiffuse, light->material.diffuse.r, light->material.diffuse.g, light->material.diffuse.b);
        }
        
        if ( LightPosition >= 0 ){
            glm::vec3 lightPosition(light->position.x,light->position.y,light->position.z);
            glUniform3fv(LightPosition, 1, (float*)&lightPosition);
        }

    }
}

// Apply material on the object
void ObjLoader::ApplyMaterial()
{
    if ( MaterialAmbient >= 0 ){
        glUniform3f(MaterialAmbient, materialObj.ambient.r, materialObj.ambient.g, materialObj.ambient.b);
    }
    
    if ( MaterialSpecular >= 0){
        glUniform3f( MaterialSpecular, materialObj.specular.r, materialObj.specular.g, materialObj.specular.b );
    }
    
    if ( MaterialDiffuse >= 0 ){
        glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
        glUniform3f( MaterialDiffuse, materialObj.diffuse.r, materialObj.diffuse.g, materialObj.diffuse.b );
    }
    
    if ( MaterialShininess >= 0 ){
        glUniform1f(MaterialShininess, materialObj.shiness);
    }
}


bool ObjLoader::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
{
    int index0, index1, index2;

    // COMPUTE EACH TRIANGLE AND PERFORM INTERSECTION.
    for( uint i = 0; i < objMeshModel->vertices.size(); i += 3 )
    {
        vec4 p0 = vec4(objMeshModel->vertices.at(i).position, 1.0);
        vec4 p1 = vec4(objMeshModel->vertices.at(i+1).position, 1.0);
        vec4 p2 = vec4(objMeshModel->vertices.at(i+2).position, 1.0);

        p0 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p0;
        p1 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p1;
        p2 = *TransformObj->TransformGetModelMatrix()*GetEyeCoordinatesFromRoot() * p2;
        if ( glm::intersectLineTriangle(ray0.destination, ray0.direction, vec3(p0.x,p0.y,p0.z), vec3(p1.x,p1.y,p1.z), vec3(p2.x,p2.y,p2.z), intersectionPoint)){
            return true;
        }
    }
    return false;
}

void ObjLoader::TouchEventDown( float x, float y )
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
        //printf("\nTYN:%s\n", typeid(this).name());
    }

    Model::TouchEventDown(x,y);
}

void ObjLoader::TouchEventMove( float x, float y )
{
}

void ObjLoader::TouchEventRelease( float x, float y )
{
}
