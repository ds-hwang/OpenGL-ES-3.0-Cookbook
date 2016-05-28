#include "3DSLoader.h"
#include <lib3ds/file.h>
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>
#include "Cache.h"

#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"vertex3ds.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"frag3ds.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/vertex3ds.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/frag3ds.glsl"
#endif

#define VERTEX_POSITION 0
#define VERTEX_NORMAL 1
#define VERTEX_TEXCOORD 2

char UniformKa;
char UniformKd;
char UniformKs;
char UniformKsh;
char UniformMVP;

/*!
	Loader3DS constructor.

	\param[in] modelType Type of model rendered.
	\param[in] parent Parent handle.
*/
Loader3DS::Loader3DS( Renderer* parent)
{
	if (!parent)
		return;

	RenderHandler       = parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj        = parent->RendererTransform();
	modelType			= ThreeDSFileType;
    RenderPrimitive     = 2;

    char fname[MAX_PATH]= {""};
    glEnable	( GL_DEPTH_TEST );

#ifdef __IPHONE_4_0
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/Models/" );
#endif

    strcat( fname, "car.3ds" );

    file = load3dsModel(fname);

}

/*!
	Destructor Loader3DS.

	\param[in] VOID.
*/
Loader3DS::~Loader3DS () {
	if ( PROGRAM* program = ProgramManagerObj->Program( (char*)"3dsShader" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}


/*!
    Initialize the resource of 3DS Model. This function is initializing the vertex and fragment shader files.

	\param[in] VOID.
*/
void Loader3DS::InitModel()
{
	ProgramManagerObj = RenderHandler->RendererProgramManager();
	if (! ( program = ProgramManagerObj->Program( ( char * )"3dsShader" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"3dsShader" );
		ProgramManagerObj->AddProgram( program );
	}
	else{
		return;
	}

    // Initialize the vertex and shader source files.
	program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER );
	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER );

	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
        m = mclose( m );
	}

	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        m = mclose( m );
	}

    if( !ProgramManagerObj->ProgramLink( program, 1 ) )
        exit( 3 );

    glUseProgram( program->ProgramID );

    UniformKa = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"Ka");
    UniformKd = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"Kd");
    UniformKs = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"Ks");
    UniformKsh = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"Ksh");
    UniformMVP = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"MODELVIEWPROJECTIONMATRIX");
}

void Loader3DS::TouchEventDown( float x, float y )
{
    RenderPrimitive++;
    if (RenderPrimitive > 2)
        RenderPrimitive = 0;
}

void Loader3DS::TouchEventMove( float x, float y )
{
}

void Loader3DS::TouchEventRelease( float x, float y )
{
}


/*!
	This function load the 3ds binary file and parse the information into the structure.

	\param[in] fileName path of the 3ds model filename.
*/
Lib3dsFile* Loader3DS::load3dsModel(const char* fileName)
{
	Lib3dsFile* file = lib3ds_file_load(fileName);
	if (!file) {
		LOGI("***ERROR***\nLoading file %s failed\n", fileName);
		return NULL;
	}

	if( !file->nodes )
	{
		Lib3dsMesh* mesh = NULL;
		Lib3dsNode* node = NULL;

		for(mesh = file->meshes; mesh != NULL; mesh = mesh->next)
		{
			node = lib3ds_node_new_object();
			strcpy(node->name, mesh->name);
			node->parent_id = LIB3DS_NO_PARENT;
			lib3ds_file_insert_node(file, node);
		}
	}

	return file;
}

void Loader3DS::SetMaterialInfo( Lib3dsMesh *mesh )
{
    Lib3dsMaterial *material = 0;
    if ( mesh->faces ) {
        material = lib3ds_file_material_by_name(file, mesh->faceL[0].material);
    }

    if( !material ){
        return;
    }

    glUniform4f(UniformKa, material->ambient[0], material->ambient[1], material->ambient[2], material->ambient[3]);
    glUniform4f(UniformKd, material->diffuse[0], material->diffuse[1], material->diffuse[2], material->diffuse[3]);
    glUniform4f(UniformKs, material->specular[0], material->specular[1], material->specular[2], material->specular[3]);
    glUniform1f(UniformKsh, material->shininess);
}

void Loader3DS::BuildMesh(Lib3dsMesh *mesh)
{
    int sizeMeshVert            = 0;
	int sizeMeshTexture         = 0;
	int sizeMeshNormal          = 0;
	int sizefaceIndex           = 0;
	int indexNumber             = 0;
	GLuint vId                  = 0;
	GLuint VAOId                = 0;
	GLuint iId                  = 0;
    int idx                     = 0;
    float* meshVert             = NULL;
    float* meshTexture          = NULL;
    Lib3dsVector *meshNormal    = NULL;
    unsigned short* faceIndex   = NULL;
    MyLib3dsUserData* userObj   = (MyLib3dsUserData*)mesh->user.p;

    // Allocate memory for vertex positions
    meshVert = new float[ mesh->points * 3 ];
    for(int k=0; k<mesh->points; k++)
    {
        meshVert[idx++] = mesh->pointL[k].pos[0];
        meshVert[idx++] = mesh->pointL[k].pos[1];
        meshVert[idx++] = mesh->pointL[k].pos[2];
    }

    // Allocate memory for texture
    meshTexture = new float[ mesh->texels * 2 ];
    for(int k=0, idx = 0; k<mesh->texels; k++)
    {
        meshTexture[idx++] = (float) mesh->texelL[k][0];
        meshTexture[idx++] = (float) mesh->texelL[k][1];
    }

    // Allocate memory for normal
    meshNormal = new Lib3dsVector[ 3 * mesh->faces ];
    lib3ds_mesh_calculate_normals(mesh, meshNormal);

    // Allocate memory for face information 3 indices make one face
    faceIndex = new unsigned short[mesh->faces*3];
    for(int k=0,idx = 0; k<mesh->faces; k++)
    {
        faceIndex[idx++] = mesh->faceL[k].points[0];
        faceIndex[idx++] = mesh->faceL[k].points[1];
        faceIndex[idx++] = mesh->faceL[k].points[2];
    }

    int faceIndexNum    = 0;
    faceIndexNum        = mesh->faces  * 3;
    userObj->strip      = 0; // no stripfication supported for now.

    sizeMeshVert	= sizeof( float )			* mesh->points * 3;
    sizeMeshTexture	= sizeof( float )			* mesh->texels * 2;
    sizefaceIndex	= sizeof( unsigned short )	* faceIndexNum;
    sizeMeshNormal  = sizeof( Lib3dsVector )	* mesh->faces  * 3;
    indexNumber		= faceIndexNum;

    // Create and Bind the VBO. Fill the VBO with Vertex position, texture, normal information.
    glGenBuffers( 1, (GLuint *)&vId );
    glBindBuffer( GL_ARRAY_BUFFER, vId );
    glBufferData( GL_ARRAY_BUFFER, sizeMeshVert + sizeMeshTexture, 0, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0,			sizeMeshVert,	meshVert );
    glBufferSubData( GL_ARRAY_BUFFER, sizeMeshVert, sizeMeshTexture, meshTexture );
    glBufferSubData( GL_ARRAY_BUFFER, sizeMeshVert + sizeMeshTexture, sizeMeshNormal, meshNormal );

    // Create and Bind the IBO. Fill the IBO with index information.
    glGenBuffers( 1, (GLuint *)&iId );
    glBindBuffer( GL_ARRAY_BUFFER, iId );
    glBufferData( GL_ARRAY_BUFFER, sizefaceIndex, 0, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizefaceIndex, faceIndex );

    // Create and Bind Vertex Array Object
    glGenVertexArrays(1, &VAOId);
    glBindVertexArray(VAOId);
    glBindBuffer( GL_ARRAY_BUFFER, vId );
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(VERTEX_NORMAL);
    glEnableVertexAttribArray(VERTEX_TEXCOORD);
    glVertexAttribPointer( VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    glVertexAttribPointer( VERTEX_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, (void*)(int)userObj->vertexSize );
    glVertexAttribPointer( VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (void*)(int)userObj->normalOffset );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
    glBindVertexArray(0);

    // Cache the information in the User data structure
    userObj->indexNum     = indexNumber;
    userObj->vertexSize	  = sizeMeshVert;
    userObj->normalOffset = sizeMeshVert + sizeMeshTexture;
    userObj->vertexId	  = vId;
    userObj->indexId	  = iId;
    userObj->VAOId		  = VAOId;
    userObj->textureId    = 0;

    delete []faceIndex;
    delete []meshVert;
    delete []meshTexture;
    delete []meshNormal;
}

/*!
	This function responsible for rendering a 3ds model. The current approach support nested models
	each model can be rendered using vertex buffer for high optimization.

	The .3ds model do have the capability to render the model without having texture bitmap,
	these models are capable of render the same model with color information.
	This way new models can be supported without dependency on texture based models, these models will be faster load and process.

	Requirements implemented:-

	1. Understanding of the color information embedded in the 3ds binary format.
	2. Modification of the existing 3DMax shader to support the color info in absence of texture, the shader must
	   support both models with/without texture therefore the shader program.

	\param[in] file The Lib3dsFile file structure containing complete parsed information for 3ds model.
*/
void Loader3DS::RenderNodes(Lib3dsFile* file, Lib3dsNode *node)
{
	if (!file || !node)
		return;

    // Use 3DS Loader Program
	glUseProgram( program->ProgramID );

    // Render all node iteratively.
    Lib3dsNode *tempNode;
    for ( tempNode=node->childs; tempNode!=0; tempNode=tempNode->next ) {
		RenderNodes(file, tempNode);
    }

	if ( node->type != LIB3DS_OBJECT_NODE || strcmp(node->name,"$$$DUMMY")==0) {
        return;
    }

    Lib3dsMesh *mesh;
    mesh = lib3ds_file_mesh_by_name(file, node->data.object.morph);
    if( mesh == NULL ){
        mesh = lib3ds_file_mesh_by_name(file, node->name);
    }

    // If invalid mesh return.
    if (!mesh) {
        return;
    }

    if (!mesh->user.p){
        MyLib3dsUserData* userObject = new MyLib3dsUserData;
        mesh->user.p = (void*)userObject;
    }

    MyLib3dsUserData* userObj = (MyLib3dsUserData*)mesh->user.p;

    if ( !userObj->VAOId ) {
        BuildMesh( mesh );
    }
    else {
        // Apply Transformation.
        TransformObj->TransformPushMatrix();
        TransformObj->TransformRotate(-90, 1, 0, 0);
        TransformObj->TransformScale(0.0135, 0.0135, 0.0135);
        TransformObj->TransformTranslate
                    (node->data.object.pivot[1], node->data.object.pivot[0], node->data.object.pivot[2]);
        glUniformMatrix4fv
                    (UniformMVP,1,GL_FALSE,(float*)TransformObj->TransformGetModelViewProjectionMatrix());
        TransformObj->TransformPopMatrix();

        // Set Material information
        SetMaterialInfo( mesh );

        //Bind to VAO
        glBindVertexArray(userObj->VAOId);

        if ( RenderPrimitive == 0 ){
            glDrawElements(GL_POINTS, userObj->indexNum, GL_UNSIGNED_SHORT, (void*)0);
        }
        else if ( RenderPrimitive == 1 ){
            glDrawElements(GL_LINES, userObj->indexNum, GL_UNSIGNED_SHORT, (void*)0);
        }
        else{
            glDrawElements(GL_TRIANGLES, userObj->indexNum , GL_UNSIGNED_SHORT, (void*)0);
        }

        //Bind to default VAO
        glBindVertexArray(0);
    }
	glFlush();
}


/*!
	This function responsible for rendering a 3ds model.
	It recursively calls the subchilds of the 3ds file called nodes.

	\param[in] file The Lib3dsFile file structure containing complete parsed information for 3Ds model.
*/
void Loader3DS::Render(Lib3dsFile* file)
{
	if (!file)
		return;

	// Set the rendering opengl state
	setStates();

	TransformObj->TransformPushMatrix();
	Lib3dsNode *nodeElement;
	for ( nodeElement = file->nodes; nodeElement != 0; nodeElement = nodeElement->next )
	{
		RenderNodes( file, nodeElement );
	}
	TransformObj->TransformPopMatrix();

	// Reset the rendering opengl state
	releaseStates();

	return;
}

/*!
	This function set the states required for rendering of 3Ds models.

	\param[in] None
*/
void Loader3DS::setStates()
{
	glEnable	( GL_DEPTH_TEST );
	glDepthFunc	( GL_LEQUAL );
	glDepthMask	( GL_TRUE ); // This makes the depth working very important.
}

/*!
	This function reset the states prior to the rendering of 3Ds models.

	\param[in] None
*/
void Loader3DS::releaseStates()
{
	glDisable( GL_DEPTH_TEST );
}

/*!
	This function release all allocated resources.
    Delete VBO's and mesh data structure

	\param[in] file 3Ds file structure containing model info.
	\return    not in use at present.
*/
bool Loader3DS::clearModel(Lib3dsFile* file)
{
	if (!file)
		return false;

	Lib3dsMesh *mesh;
    MyLib3dsUserData* userObj = (MyLib3dsUserData*)mesh->user.p;
	for ( mesh = file->meshes; mesh != 0; mesh = mesh->next )
	{
        // Delete the Vertex Buffer Objects VBO's
		if ( userObj->vertexId ){
			glDeleteBuffers( 1, (GLuint *)&userObj->vertexId );
			userObj->vertexId = 0;
		}

		if ( userObj->indexId ){
			glDeleteBuffers( 1, (GLuint *)&userObj->indexId );
			userObj->indexId = 0;
		}

		if ( userObj->textureId ){
			glDeleteTextures( 1, (GLuint *)&userObj->textureId );
			userObj->textureId = 0;
		}

		if ( userObj->VAOId ){
			glDeleteVertexArrays( 1, (GLuint *)&userObj->VAOId );
			userObj->VAOId = 0;
		}
	}

    for ( Lib3dsMesh* mesh = file->meshes; mesh; mesh = mesh->next ) {
      lib3ds_mesh_free( mesh );
    }
	return true;
}

void Loader3DS::Render()
{
    Render(file);
}

