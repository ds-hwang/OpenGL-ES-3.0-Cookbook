#include "HeightField.h"
#include "Cache.h"

#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"HeightFldVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"HeightFldFragment.glsl"
#else
#define VERTEX_SHADER_PRG			( char * )"shader/HeightFldVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"shader/HeightFldFragment.glsl"
#endif


#define VERTEX_POSITION 0
#define NORMAL_POSITION 1
#define TEX_COORD 2
#define COLOR 3

bool toggle = true;


HeightField::HeightField(Renderer* parent, float rDimension, float cDimension, int Rows, int Columns)
{
	if (!parent)
    return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();
	modelType 			= ObjFileType;
    
    char fname[MAX_PATH]= {""};

    // Load Height Map image
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/Images/" );
#endif
    strcat( fname, "HeightMap.png" );
    //http://blogs.msdn.com/blogfiles/shawnhar/WindowsLiveWriter/AppWeekBuildinglevels_F053/image_93a8f6b9-26bd-4288-b4ee-8dd6840c515e.png
    //strcat( fname, "HeightMap2.png" );
    //http://en.wikipedia.org/wiki/Heightmap#mediaviewer/File:Heightmap.png
    imageHeightMap = new PngImage();
    imageHeightMap->loadImage(fname);
    
    // Load grass image
    memset(fname, 0, MAX_PATH);
#ifdef __APPLE__
    GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );
#else
    strcpy( fname, "/sdcard/Images/" );
#endif
    strcat( fname, "grass.png" );
    //http://forum.thesims3.com/jforum/posts/list/60/151396.page

    imageGrass = new PngImage();
    imageGrass->loadImage(fname);
    

	vId = iId = 0;
    faces = Rows * Columns;
    v	= new float[3 * (Rows + 1) * (Columns + 1)];
    n	= new float[3 * (Rows + 1) * (Columns + 1)];
    tex = new float[2 * (Rows + 1) * (Columns + 1)];
    faceIdx	= new  unsigned short [6 * Rows * Columns];
	sizeofVertex	= sizeof( float ) * 3 * (Rows + 1) * (Columns + 1);
    sizeofTex		= sizeof( float ) * 2 * (Rows + 1) * (Columns + 1);
	sizeofFace		= sizeof( unsigned short ) * 6 * Rows * Columns ;
	
	float x2        = rDimension / 2.0f;
    float z2        = cDimension / 2.0f;
    float zFactor   = (float)cDimension / Columns;
    float xFactor   = (float)rDimension / Rows;
    float texi      = 1.0f / Columns;
    float texj      = 1.0f / Rows;

    float x, z;
    int vidx = 0, tidx = 0;
    // Calculate the Vertices,Normal and Texture Coords
    for( int i = 0; i <= Columns; i++ ) {
        z = zFactor * i - z2; // Columns
        for( int j = 0; j <= Rows; j++ ) {
            x = xFactor * j - x2; // Rows
            
            // Vertices
            v[vidx]     = x;
            v[vidx+1]   = 0.0f;
            v[vidx+2]   = z;
            
            // Normals: assuming all normal are along +Y direction
            n[vidx]     = 0.0f;
            n[vidx+1]   = 1.0f;
            n[vidx+2]   = 0.0f;

            vidx += 3;
            
            // Texture coordinates
            tex[tidx]   = j * texj;
            tex[tidx+1] = i * texi;
            tidx += 2;
        }
    }

    unsigned int rowStart, nextRowStart;
    int idx = 0;
    // Calculate the face indices
    for( int i = 0; i < Columns; i++ ) {
        rowStart = i * (Rows+1);
        nextRowStart = (i+1) * (Rows+1);
        for( int j = 0; j < Rows; j++ ) {
            faceIdx[idx]    = rowStart + j;
            faceIdx[idx+1]  = nextRowStart + j;
            faceIdx[idx+2]  = nextRowStart + j + 1;
            faceIdx[idx+3]  = rowStart + j;
            faceIdx[idx+4]  = nextRowStart + j + 1;
            faceIdx[idx+5]  = rowStart + j + 1;
            idx += 6;
        }
    }

    // Generate and bind the Vertex Buffer Object
    glGenBuffers(1, &vId);
	glBindBuffer( GL_ARRAY_BUFFER, vId );
	glBufferData( GL_ARRAY_BUFFER, sizeofVertex + sizeofVertex + sizeofTex, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0,			sizeofVertex,	v );
	glBufferSubData( GL_ARRAY_BUFFER, sizeofVertex,			sizeofVertex,	n );
	glBufferSubData( GL_ARRAY_BUFFER, sizeofVertex*2, sizeofTex,	  tex );

    // Generate and bind the Index Buffer Object
    glGenBuffers(1, &iId);
    glBindBuffer( GL_ARRAY_BUFFER, iId);
	glBufferData( GL_ARRAY_BUFFER, sizeofFace, 0, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeofFace, faceIdx );
    
    // Create the Vertex Array object for height field
    glGenVertexArrays(1, &HeightFieldVAO_Id);
    glBindVertexArray(HeightFieldVAO_Id);
    
    // Bind the VBO for VAO
    glBindBuffer( GL_ARRAY_BUFFER, vId );
    glEnableVertexAttribArray(VERTEX_POSITION);     // Vertex	position
    glEnableVertexAttribArray(NORMAL_POSITION);     // Normal	position
    glEnableVertexAttribArray(TEX_COORD);           // Texture	position
    
    glVertexAttribPointer( VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    glVertexAttribPointer( NORMAL_POSITION, 3, GL_FLOAT, GL_FALSE, 0,(void*)sizeofVertex );
    glVertexAttribPointer( TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeofVertex*2) );
    
    // Bind the IBO for VAO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iId );
    glBindVertexArray(0);

    // Delete temporary buffer
    if(v != NULL)
    {
        delete [] v;
        v = NULL;
    }
    
    if(n != NULL)
    {
        delete [] n;
        n = NULL;
    }
    
    if(tex != NULL)
    {
        delete [] tex;
        tex = NULL;
    }

    if(faceIdx != NULL)
    {
        delete [] faceIdx;
        faceIdx = NULL;
    }
    
}

HeightField::~HeightField()
{
	if(v != NULL)
	{
		delete [] v;
		v = NULL;
	}
    if(n != NULL)
    {
    	delete [] n;
    	n = NULL;
    }
    if(tex != NULL)
    {
    	delete [] tex;
    	tex = NULL;
    }
    
    if(faceIdx != NULL)
    {
    	delete [] faceIdx;
    	faceIdx = NULL;
    }
	
	if (vId){
		glDeleteBuffers(1, &vId);
	}
    
	if (iId){
		glDeleteBuffers(1, &iId);
	}
}

void HeightField::InitModel()
{
	if (! ( program = ProgramManagerObj->Program( ( char * )"Grid" ) ) )
	{
		program = ProgramManagerObj->ProgramInit( ( char * )"Grid" );
		ProgramManagerObj->AddProgram( program );
	}
    
	program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER );
	program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER );
    
    /////////// Vertex shader //////////////////////////
	CACHE *m = reserveCache( VERTEX_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
        m = mclose( m );
	}
    
    /////////// Fragment shader ////////////////////////
	m = reserveCache( FRAGMENT_SHADER_PRG, true );
	if( m ) {
		if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
        m = mclose( m );
	}
    
    if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );
    
    glUseProgram( program->ProgramID );
    
    // Get the location of the ImageTexture variable defined in the vertex shader and bind it with
    // 0th texture unit.
    TEX_HEIGHT = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "HeightMapTexture" );
    glActiveTexture (GL_TEXTURE0);
    if (imageHeightMap) {
        glBindTexture(GL_TEXTURE_2D, imageHeightMap->getTextureID());
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    // Get the location of the ImageGrassTexture variable defined in the fragment shader and bind it with
    // 0th texture unit.
    TEX_GRASS = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "ImageGrassTexture" );
    glActiveTexture (GL_TEXTURE1);
    if (imageGrass) {
        glBindTexture(GL_TEXTURE_2D, imageGrass->getTextureID());
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    MVP = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectionMatrix" );
    FACTOR = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"heightFactor" );
    if ( FACTOR >= 0 ){
        glUniform1f(FACTOR, 2.0);
    }

    return;
}

void HeightField::Render()
{
    glUseProgram( program->ProgramID );
    glDisable(GL_CULL_FACE);

    glUniform1i(TEX_HEIGHT, 0);
    glUniform1i(TEX_GRASS, 1);
    
    TransformObj->TransformPushMatrix();
    TransformObj->TransformRotate(rot++ , 0.0, 1.0, 0.0);
    glUniformMatrix4fv( MVP, 1, GL_FALSE,(float*)TransformObj->TransformGetModelViewProjectionMatrix() );

    // Bind with Vertex Array Object for HeightField
    glBindVertexArray(HeightFieldVAO_Id);
    if ( toggle )
        glDrawElements(GL_TRIANGLES, 6 * faces, GL_UNSIGNED_SHORT, (void*)0);
    else
        glDrawElements(GL_LINES, 6 * faces, GL_UNSIGNED_SHORT, (void*)0);
    TransformObj->TransformPopMatrix();
}

void HeightField::TouchEventDown( float a, float b )
{
    toggle = !toggle;
}
