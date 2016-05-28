#include "ObjLoader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
#import <fstream>
using namespace glm;

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"ADSVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"ADSFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/ADSVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/ADSFragment.glsl"
#endif


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

	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= ObjFileType;
    char fname[MAX_PATH]= {""};
    glEnable	( GL_DEPTH_TEST );

	#ifdef __IPHONE_4_0
		GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
    #else
	    strcpy( fname, "/sdcard/Models/" );
    #endif

    //strcat( fname, "Sphere.obj" );
    //strcat( fname, "Cone.obj" );
    //strcat( fname, "Torus.obj" );
    strcat( fname, "SDMonkey.obj" );
    //strcat( fname, "IsoSphere.obj" );
    //strcat( fname, "Cylinder.obj" );

    PareObjMesh(fname);
    

    // Create the VBO for our obj model vertices.
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, objMeshModel.vertices.size() * sizeof(objMeshModel.vertices[0]), &objMeshModel.vertices[0], GL_STATIC_DRAW);
    objMeshModel.vertices.clear();
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
	if ( program = ProgramManagerObj->Program( ( char * )"ADSShader" ) )
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


	if (! ( program = ProgramManagerObj->Program( ( char * )"ADSShader" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"ADSShader" );
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
	}
	m = mclose( m );

	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
	}

	m = mclose( m );

    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
    
    char MaterialAmbient   = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"MaterialAmbient");
    char MaterialSpecular  = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"MaterialSpecular");
    char MaterialDiffuse   = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"MaterialDiffuse");
    char LightAmbient      = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"LightAmbient");
    char LightSpecular     = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"LightSpecular");
    char LightDiffuse      = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"LightDiffuse");
    char ShininessFactor   = ProgramManagerObj->ProgramGetUniformLocation(program,      (char*)"ShininessFactor");
    
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
    
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void ObjLoader::Render()
{
	Transform*	TransformObj = MapRenderHandler->RendererTransform();

	TransformObj->TransformPushMatrix();
	//TransformObj->TransformTranslate(0,0,-3);
	//TransformObj->TransformScale(1.0,1.0,1.0);
    static float rot = 0.0;
	TransformObj->TransformRotate(rot++ , 0.0, 1.0, 0.0);

    glUseProgram(program->ProgramID);
	char uniform = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    
	if ( uniform >= 0 )
	{
		glUniformMatrix4fv( uniform, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	}

	/*uniform = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWMATRIX" );
    
	if ( uniform >= 0 )
	{
		glUniformMatrix4fv( uniform, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	}*/
    
    int stride              = (2 * sizeof(vec3) )+ sizeof(vec2);
    const GLvoid* offset    = (const GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) );

    GLint Normal            = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"Normal");
    
    char attribVertex = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexPosition");
    if (attribVertex>=0)
    {
        glEnableVertexAttribArray(attribVertex);
        glEnableVertexAttribArray(Normal);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
        glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, stride, 0);
        glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, stride, offset);
    }

    GLint NormalMatrix  = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"NormalMatrix");
    GLint LightPosition = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
    
    glm::mat4 matrix    = *(TransformObj->TransformGetModelViewMatrix());
    glm::mat3 normalMat = glm::mat3( glm::vec3(matrix[0]), glm::vec3(matrix[1]), glm::vec3(matrix[2]) );
    glUniformMatrix3fv( NormalMatrix, 1, GL_FALSE, (float*)&normalMat );
    
    glm::vec3 lightPosition(0.0, 0.0, 10);
    glUniform3fv(LightPosition, 1, (float*)&lightPosition);
    
    glDrawArrays(GL_TRIANGLES, 0, IndexCount );
    glDisableVertexAttribArray(attribVertex);
    
	TransformObj->TransformPopMatrix();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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


bool ObjLoader::PareObjMesh( char* path )
{
    FILE*  pFile;
    bool   eofReached = false;
    char   c;
    float  x, y, z, u, v;
    float  scanCount;
    int    vIndex, nIndex, uvIndex;
    
    pFile = fopen ( path,"r" );
    if( pFile==NULL )
    {
        fputs ("fopen ",pFile);
        LOGI("Fail to open Obj mesh file: %s", path);
        fclose (pFile);
        return false;
    }
    
    // Face should in the form => v/u/n
    strcpy(objMeshModel.fileName, path);
    while(!eofReached)
    {
        c = fgetc(pFile);
        switch(c)
        {
            case '#': // Comment. Ignore
            case 'u': // Ignore
            case 's': // Ignore
            case 'g': // Grouping not supported
                while(fgetc(pFile) != '\n'); // Skip till new next line not reached.
                break;
            
            case EOF: // End of file.
                eofReached = true;
                break;
                
            case 'v': // Loading the vertices.
                c = fgetc(pFile); // The next character will let us know what vertex attribute to load
                switch(c)
                {
                    case ' ': // Loading vertices
                        fscanf(pFile,"%f %f %f\n",&x,&y,&z);
                        objMeshModel.positions.push_back(glm::vec3(x, y, z));
                        break;
                    case 'n': // Loading normals
                        fscanf(pFile,"%f %f %f\n",&x,&y,&z);
                        objMeshModel.normals.push_back(glm::vec3(x, y, z));
                        break;
                    case 't': // Loading UVs
                        fscanf(pFile,"%f %f\n",&u,&v);
                        objMeshModel.uvs.push_back(glm::vec2(u, v));
                        break;
                    default:
                        return false;
                }
                break;
            case 'f':// 'f' means it is a face index information in the form of v/u/n
                // If Normal and UV information available. Format => v/u/n
                if ( objMeshModel.normals.size() && objMeshModel.uvs.size() )
                {

                    while(fgetc(pFile) != '\n'){
                        vIndex = nIndex = uvIndex = 0;
                    
                        scanCount = fscanf(pFile,"%d/%d/%d",&vIndex,&uvIndex,&nIndex);
                        if((scanCount != 3) || (vIndex == 0) || (uvIndex == 0) || (nIndex == 0))
                        {
                            break;
                        }
                    
                        objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
                    }
                }
                // If Normal information available and UV missing. Format => v//n
                else if( objMeshModel.normals.size() && !objMeshModel.uvs.size() )
                {
                    while(fgetc(pFile) != '\n'){
                        vIndex = nIndex = uvIndex = 0;
                        scanCount = fscanf(pFile,"%d//%d",&vIndex, &nIndex);
                        if((scanCount != 2) || (vIndex == 0) || (nIndex == 0))
                        {
                            break;
                        }
                    
                        objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
                    }
                }
                // If Normal information missing and UV available. Format => v/u/
                else if( !objMeshModel.normals.size() && objMeshModel.uvs.size() )
                {
                    while(fgetc(pFile) != '\n'){
                        vIndex = nIndex = uvIndex = 0;
                        scanCount = fscanf(pFile,"%d/%d/",&vIndex, &uvIndex);
                        if((scanCount != 2) || (vIndex == 0) || (uvIndex == 0))
                        {
                            break;
                        }
                    
                        objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
                    }
                }
                // If both Normal and UV information missing. Format => v
                else if( !objMeshModel.normals.size() && !objMeshModel.uvs.size() )
                {
                    vIndex = nIndex = uvIndex = 0;
                    while(fgetc(pFile) != '\n'){
                        scanCount = fscanf(pFile,"%d",&vIndex);
                        if((scanCount != 1) || (vIndex == 0))
                        {
                            break;
                        }
                        objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
                    }
    
                }
                break;
        }
    }
    
    objMeshModel.vertices.resize(objMeshModel.vecFaceIndex.size());
    objMeshModel.indices.resize(objMeshModel.vecFaceIndex.size());

    // Get the total number of indices.
    IndexCount = objMeshModel.indices.size();

    //Create the interleaved vertex information for Vertex containing position, uv and normal.
    for(int i = 0; i < objMeshModel.vecFaceIndex.size(); i++)
    {
        //Position information must be available always
        int index = objMeshModel.vecFaceIndex.at(i + 0).vertexIndex;
        objMeshModel.vertices[i].position = objMeshModel.positions.at(index);
        objMeshModel.indices[i] = (GLushort)objMeshModel.vecFaceIndex.at(i).vertexIndex;
        
        // If UV information is available.
        if(objMeshModel.uvs.size()){
            index = objMeshModel.vecFaceIndex.at(i).uvIndex;
            objMeshModel.vertices[i].uv = objMeshModel.uvs.at(index);
        }
        
        // If Normal information is available.
        if(objMeshModel.normals.size())
        {
            index = objMeshModel.vecFaceIndex.at(i ).normalIndex;
            objMeshModel.vertices[i].normal = objMeshModel.normals.at(index);
        }
    }
    
    // If normal information is missing from the OBJ file then it could be calculated using the position
    // and face index information. It can be either calculated in Flat Shading or Smooth Shading.
    if( objMeshModel.normals.size() == 0 ){
        // Boolean flag to control type of shading information required.
        // Smooth Shading: Common vertice among faces share unique normal which is calculated after taking resultant of all the normal among those faces. These are called vertex normals.
        // Flat Shading: All face have there own normal which are basically face normals.
        bool flatShading = false;

        objMeshModel.normals.resize(objMeshModel.positions.size());
        int index0, index1, index2;
        for(int i=0; i<objMeshModel.indices.size();i += 3)
        {
            index0 = objMeshModel.indices.at(i);
            index1 = objMeshModel.indices.at(i+1);
            index2 = objMeshModel.indices.at(i+2);
        
            vec3 a = objMeshModel.positions.at(index0);
            vec3 b = objMeshModel.positions.at(index1);
            vec3 c = objMeshModel.positions.at(index2);
            
            vec3 faceNormal =  glm::cross((b - a), (c - a));

            if ( flatShading ){
                objMeshModel.vertices[i].normal += faceNormal;
                objMeshModel.vertices[i+1].normal += faceNormal;
                objMeshModel.vertices[i+2].normal += faceNormal;
            }
            else
            {
                objMeshModel.normals[index0] += faceNormal;
                objMeshModel.normals[index1] += faceNormal;
                objMeshModel.normals[index2] += faceNormal;
            }
        }
        
        if ( !flatShading ){
            for(int i = 0; i < objMeshModel.vecFaceIndex.size(); i++)
            {
                int index = objMeshModel.vecFaceIndex.at(i + 0).vertexIndex;
                objMeshModel.vertices[i].normal = objMeshModel.normals.at(index);
            }
        }
    
        for (int j=0;j<objMeshModel.vertices.size(); j++){
            objMeshModel.vertices[j].normal = glm::normalize(objMeshModel.vertices[j].normal);
        }
    }
    
    objMeshModel.positions.clear();
    objMeshModel.normals.clear();
    objMeshModel.uvs.clear();
    objMeshModel.indices.clear();
    objMeshModel.vecFaceIndex.clear();

    fclose (pFile);
    return true;
}
