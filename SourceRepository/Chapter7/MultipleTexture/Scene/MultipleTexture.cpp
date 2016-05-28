#include "MultipleTexture.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

//Note: The Linux is very case sensitive so be aware of specifying correct folder and filename.
#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"MultipleTextureVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"MultipleTextureFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/MultipleTextureVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/MultipleTextureFragment.glsl"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

static float rotationX = 0.0;
static float rotationY = 0.0;
static float lastX = 0;
static float lastY = 0;
#define PACE 1.0
static float deltaX = 0;
static float deltaY = 0;
static float DecelerationFactor = 0.5;

float texCoords[8] = { 0.0f, 1.0f,
                        1.0f, 1.0f,
                        0.0f, 0.0f,
                        1.0f, 0.0f
                     };

float quad[12]     = {
                        -1.0f, -1.0f,  0.0f,
                         1.0f, -1.0f,  0.0f,
                        -1.0f, 1.0f, -0.0f,
                         1.0f, 1.0f, -0.0f
                     };

// Namespace used
using std::ifstream;
using std::ostringstream;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
MultipleTexture::MultipleTexture( Renderer* parent )
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= ImageDemoType;
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
MultipleTexture::~MultipleTexture()
{
	PROGRAM* program = NULL;
	if ( program = ProgramManagerObj->Program( ( char * )"square" ) )
	{
		ProgramManagerObj->RemoveProgram(program);
	}
}
char* MultipleTexture::creatPath(char* fname, char* fileName) {
    
    char path[MAX_PATH]= {""};
#ifdef __IPHONE_4_0
    GLUtils::extractPath( getenv( "FILESYSTEM" ), path );
#else
    strcpy( path, "/sdcard/GLPiFramework/Images/" );
#endif
    memset( fname, 0, MAX_PATH);
    strcpy( fname, path );
    strcat( fname, fileName );
    return fname;
}

void MultipleTexture::loadMultiTexture()
{
    glUseProgram( program->ProgramID );
    
    // Query the maximum number of texture units.
    int texture_units;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_units);
    TEX  = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "TexFragile" );
    TEX2 = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "TexWood" );

    // 1. Activate Texture Unit
    // 2. Generate and Bind named texture
    // 3. Load image and apply texture filter
    // 4. Query uniform samplers and set the texture unit IDs.
    
	char fname[MAX_PATH]= {""};
    glActiveTexture (GL_TEXTURE1);
    image = new PngImage();
    image->loadImage(creatPath(fname, (char*)"fragile.png")); // http://icons.mysitemyway.com/legacy-icon/115457-magic-marker-icon-food-beverage-drink-glass4-sc44/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glUniform1i(TEX, 1);
    
    glActiveTexture(GL_TEXTURE2);
    image->loadImage(creatPath(fname, (char*)"woodenBox.png")); //http://2add2equals5.wordpress.com/textures
    image2 = new PngImage();
    image2->loadImage(fname);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glUniform1i(TEX2, 2);
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void MultipleTexture::InitModel()
{

	if (! ( program = ProgramManagerObj->Program( ( char * )"square" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"square" );
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
    
    glUseProgram( program->ProgramID );
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"MODELVIEWPROJECTIONMATRIX" );
    loadMultiTexture();
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void MultipleTexture::Render()
{

    glUseProgram(program->ProgramID);

    // Disable culling
    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
	TransformObj->TransformPushMatrix();
	TransformObj->TransformRotate(rotationX, 0.0, 1.0, 0.0);
	TransformObj->TransformRotate(rotationY, 1.0, 0.0, 0.0);

    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
	glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, quad);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->TransformGetModelViewProjectionMatrix() );
	TransformObj->TransformPopMatrix();
    
    // Draw triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Manage rotation
    if ( deltaX != 0.0 ){
        if( deltaX > 0.0 ){
            deltaX -= DecelerationFactor;
        }
        else{
            deltaX += DecelerationFactor;
        }
        rotationX += deltaX;
    }

    if ( deltaY != 0.0 ){
        if( deltaY > 0.0 ){
            deltaY -= DecelerationFactor;
        }
        else{
            deltaY += DecelerationFactor;
        }
        rotationY += deltaY;
    }
}

void MultipleTexture::TouchEventDown( float x, float y )
{
    lastX = x;
    lastY = y;
}

void MultipleTexture::TouchEventMove( float x, float y )
{
    rotationX += (x - lastX) * PACE;
    rotationY += (y - lastY) * PACE;
    lastX = x;
    lastY = y;
}

void MultipleTexture::TouchEventRelease( float x, float y )
{
    deltaX = x - lastX;
    deltaY = y - lastY;
}



