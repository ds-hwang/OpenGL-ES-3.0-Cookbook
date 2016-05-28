#include "Font.h"
#include "Cache.h"
//#include "FontGenerator.h"
//extern "C" {
//#include <ft2build.h>
//#include <freetype/freetype.h>
//#include <freetype/ftglyph.h>
//#include <freetype/ftoutln.h>
//#include <freetype/fttrigon.h>
//}

#ifdef __APPLE__
#define VERTEX_SHADER_PRG			( char * )"vertexFont.vert"
#define FRAGMENT_SHADER_PRG			( char * )"fragFont.frag"
#else
#define VERTEX_SHADER_PRG			( char * )"Shaders/vertexFont.vert"
#define FRAGMENT_SHADER_PRG			( char * )"Shaders/fragFont.frag"
#endif

#define VERTEX_POSITION 0
#define TEX_COORD 1

Font::Font(char* fileName, int Size, Renderer* parent, LanguageType Language)
{
    RendererHandler     = parent;
    ProgramManagerObj	= parent->RendererProgramManager();
    TransformObj		= parent->RendererTransform();
    languageType        = Language;
    switch(languageType)
    {
        case Arabic:
            modelType = FontArabic;
            break;
        case Thai:
            modelType = FontThai;
            break;
        case Punjabi:
            modelType = FontPunjabi;
            break;
        case Tamil:
            modelType = FontTamil;
            break;
        default:
            modelType = FontEnglish;
            break;
    }

    loadFont(fileName, 50);
}

Font::~Font() {
	PROGRAM* program = NULL;
	if ((program = ProgramManagerObj->Program((char *) "font"))) {
		ProgramManagerObj->RemoveProgram(program);
	}
}

void Font::Render() {
    /*
    TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );
    
    int w = RendererHandler->screenWidthPixel();
    int h = RendererHandler->screenHeightPixel();
    
    TransformObj->TransformLoadIdentity();
    // Left ,Right ,Bottom , Top
    TransformObj->TransformOrtho( 0, w, 0, h, -100, 100);
    
    TransformObj->TransformSetMatrixMode( VIEW_MATRIX );
    TransformObj->TransformLoadIdentity();
    
    TransformObj->TransformSetMatrixMode( MODEL_MATRIX );
    TransformObj->TransformLoadIdentity();
    */
}

void Font::InitModel() {
    if (! ( program = ProgramManagerObj->Program( ( char * )"font" ) ) )
    {
        program = ProgramManagerObj->ProgramInit( ( char * )"font" );
        ProgramManagerObj->AddProgram( program );
        program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
        program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);
        
        //////////////////////////////////////////////////////
        ///////////// Vertex shader //////////////////////////
        //////////////////////////////////////////////////////
        CACHE *m = reserveCache(VERTEX_SHADER_PRG, true);
        if (m) {
            if (!ShaderManager::ShaderCompile(program->VertexShader, (char *) m->buffer, 1))
                exit(1);
            freeCache(m);
        }
        
        //////////////////////////////////////////////////////
        ///////////// Fragment shader ////////////////////////
        //////////////////////////////////////////////////////
        m = reserveCache(FRAGMENT_SHADER_PRG, true);
        if (m) {
            if (!ShaderManager::ShaderCompile(program->FragmentShader,(char *) m->buffer, 1))
                exit(2);
            freeCache(m);
        }
        
        if (!ProgramManagerObj->ProgramLink(program, 1))
            exit(3);
    }
    else{
    }
    
    
    // Get the uniform location cache for optimization purpose
    MVP         = ProgramManagerObj->ProgramGetUniformLocation( program, (char*)"ModelViewProjectMatrix" );
    TEX         = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "Tex1" );
    FRAG_COLOR  = ProgramManagerObj->ProgramGetUniformLocation( program, (char*)"TextColor" );
}

void Font::drawGlyph(const Glyph& gi) {
    glUseProgram(program->ProgramID);

	float xmargin = float(gi.metric.width)/(float) (2*64);
	float ymargin = float(gi.metric.horiBearingY)/(float)(2*64);
    
    // Calculate the texture coordinates for the glyph rendering.
    float texCoords[8] = {
        gi.atlasX, gi.atlasY,
        gi.atlasX + texDimension, gi.atlasY,
        gi.atlasX, gi.atlasY + texDimension,
        gi.atlasX + texDimension, gi.atlasY + texDimension
    };

    // Calculate the dimension of the glyph Quad.
    float quad[12]	= {
        -0.5f, 0.5f,  0.0f,
         0.5f, 0.5f,  0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f };

	for (int i = 0; i<12;){
		quad[i]   *= (float)squareSize/2;
        quad[i+1] *= (float)squareSize/2;
        quad[i+2] *= (float)0;
		i		  += 3;
	}

    // Initialize the texture with texture unit 0
    glUniform1i(TEX, 0);
    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-xmargin, ymargin, 0.0f );
    glUniformMatrix4fv(MVP, 1, GL_FALSE, (float*)TransformObj->TransformGetModelViewProjectionMatrix());
    TransformObj->TransformPopMatrix();

    // Send the vertex and texture coordinates to the shaders
    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(TEX_COORD);
    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, quad);
    glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Font::printText(char* str, GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha) {
    // Initialize OpenGL ES States
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Use font program
	glUseProgram(program->ProgramID);
    
    // Activate Texture unit 0 and assign the altas texture
	glActiveTexture (GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlasTex);

	TransformObj->TransformPushMatrix();
    GLfloat color[4] = {Red, Green, Blue, Alpha};
    glUniform4fv(FRAG_COLOR, 1, color);

    std::vector< FT_UInt >* codePointsPtr = NULL;
    int glyph_count = 0;
    if ( !produceShape(str, codePointsPtr, glyph_count) )
    {
        LOGI("Error in producing font shape");
        return;
    }

    glyph_count = (int) codePointsPtr->size();
    FT_UInt glyph_index = 0;
    for (int i = 0; i < glyph_count; i++) {
        glyph_index = codePointsPtr->at(i);

        const Glyph& gi = glyphs[glyph_index];
        TransformObj->TransformTranslate(gi.advanceHor / 2.0, 0.0, 0.0);
        drawGlyph(gi);
    }
    TransformObj->TransformPopMatrix();
    return;
}

