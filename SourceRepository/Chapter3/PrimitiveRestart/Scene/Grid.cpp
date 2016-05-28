#include "Grid.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"GridFragment.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"GridFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/GridFragment.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/GridFragment.glsl"
#endif

#define VERTEX_LOCATION 0
#define XDimension 40
#define ZDimension 40
#define XDivision 140
#define ZDivision 140

GLuint vIdGrid;
GLuint iIdGrid;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
Grid::Grid( Renderer* parent )
{
	if (!parent)
		return;

	MapRenderHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= GridType;
    CreateGrid(XDimension, ZDimension, XDivision, ZDivision);
}

void Grid::CreateGrid(GLfloat XDim, GLfloat ZDim, int XDiv, int ZDiv)
{
    GLuint vertexNum       = 3 * (XDiv+1) * (ZDiv+1) * 2;
    GLuint indexNum        = ((XDiv+1) + (ZDiv+1)) * 2;
    GLfloat* gridVertex     = new GLfloat[ vertexNum ];
    GLushort* gridIndices    = new GLushort[ indexNum ];
    
    GLfloat xInterval = XDim/XDiv;
    GLfloat zInterval = ZDim/ZDiv;
    GLfloat xHalf     = XDim/2;
    GLfloat zHalf     = ZDim/2;
    int i =0;

    for( int j=0; j<XDiv+1; j++){
        float x = j*xInterval - xHalf;
        float y = 0;
        float z = zHalf;
        gridVertex[i++] = x;
        gridVertex[i++] = y;
        gridVertex[i++] = z;

        gridVertex[i++] = x;
        gridVertex[i++] = y;
        gridVertex[i++] = -z;
    }

    for( int j=0; j<ZDiv+1; j++){
        float x = xHalf;
        float y = 0;
        float z = j*zInterval - zHalf;
        gridVertex[i++] = -x;
        gridVertex[i++] = y;
        gridVertex[i++] = z;
        
        gridVertex[i++] = x;
        gridVertex[i++] = y;
        gridVertex[i++] = z;
    }

    i =0;
    for( int j=0; j<XDiv+1; j++ ){
        gridIndices[i++] = 2*j;
        gridIndices[i++] = 2*j+1;
    }
    for( int j=0; j<ZDiv+1; j++ ){
        gridIndices[i++] = ((XDiv+1)*2) + (2*j);
        gridIndices[i++] = ((XDiv+1)*2) + (2*j+1);
    }

    i=0;
    for( int j=0; j<(XDiv+1); j++ ){
        printf("\n V:%f, %f, %f", gridVertex[i++], gridVertex[i++], gridVertex[i++]);
        printf("\n V:%f, %f, %f", gridVertex[i++], gridVertex[i++], gridVertex[i++]);
    }
    
    i=0;
    for( int j=0; j<XDiv+1; j++ ){
        printf("\nI: %d %d", gridIndices[i++], gridIndices[i++]);
    }
    
    // Create name buffer object ID
	GLuint size = vertexNum*sizeof(float);
    glGenBuffers(1, &vIdGrid);
    
    // Create VBO
	glBindBuffer( GL_ARRAY_BUFFER, vIdGrid);
	glBufferData( GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			size,	gridVertex );
    
    // Create IBO
	unsigned short indexSize = sizeof( unsigned short )*indexNum;
    glGenBuffers(1, &iIdGrid);
	glBindBuffer( GL_ARRAY_BUFFER, iIdGrid );
	glBufferData( GL_ARRAY_BUFFER, indexSize, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, indexSize,	gridIndices );
    
    // Enable the attribute locations
    glEnableVertexAttribArray(VERTEX_LOCATION);
    
    // Unbind buffer
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    delete gridVertex;
    delete gridIndices;
}

/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
Grid::~Grid()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"GridPrg" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Grid::InitModel()
{
	if (! ( program = ProgramManagerObj->Program( (char *)"GridPrg" ))){
		program = ProgramManagerObj->ProgramInit( (char *)"GridPrg" );
		ProgramManagerObj->AddProgram( program );
	}

    //Initialize Vertex and Fragment shader
	program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);

    // Compile Vertex shader
	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
        freeCache( m );
	}

    // Compile Fragment shader
	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        freeCache( m );
	}

    // Link program
    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

    glUseProgram( program->ProgramID );
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void Grid::Render()
{
    glEnable( GL_DEPTH_TEST );

    glUseProgram( program->ProgramID );
    static int k = 0;
    TransformObj->TransformPushMatrix();
    TransformObj->TransformRotate(20, 1, 0, 0);
    //TransformObj->TransformRotate(k++, 0, 1, 0);
    TransformObj->TransformTranslate(0.0f, -2, -7);
	char uniform = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
	if ( uniform >= 0 ){
		glUniformMatrix4fv( uniform, 1, GL_FALSE,(float*)TransformObj->TransformGetModelViewProjectionMatrix() );
	}
    TransformObj->TransformPopMatrix();
    
    //Enable the Primitive Restart Index
    
    //Bind the VBO
    glBindBuffer( GL_ARRAY_BUFFER, vIdGrid );
    glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iIdGrid );
    glDrawElements(GL_LINES, ((XDivision+1) + (ZDivision+1) )*2, GL_UNSIGNED_SHORT, (void*)0);
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}


/*!
 This function handle Touch event down action.
 
 \param[in] x and y screen pixel position.
 
 \return None.
 */
void Grid::TouchEventDown( float x, float y )
{
}

