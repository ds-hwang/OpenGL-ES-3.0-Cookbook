#ifndef _FontGenerator_H
#define _FontGenerator_H

extern "C" {
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
}
#include <map>
#include "Renderer.h"
class FontRenderer;

struct Glyph {
    FT_Glyph_Metrics metric;
    float leftMargin, topMargin;
    // horizontal advance
    float advanceHor;
    float texCoordX, texCoordY;
    // tex coordinates in the texture atlas
    float atlasX, atlasY;
};

class FontGenerator {
    
  public:
    FontGenerator ();

    ~FontGenerator ();

    bool errorState ();

    bool loadFont(const char* filename, int resolution);
	
    GLuint                          atlasTex;
    std::map<unsigned long, Glyph>  glyphs;
    float                           texDimension;
    float                           squareSize;
    LanguageType                    languageType;
    FT_Face                         fontFace;
    
private:
    bool readFont (const FT_Face& fontFace, int resolution, int glyphMargin);

    bool getCorrectResolution(const FT_Face& fontFace, int resolution, int& newResolution, int& newMargin);
    
    void generateTexFromGlyph (FT_GlyphSlot glyph, GLubyte* texture, int atlasX, int atlasY, int texSize, int resolution, int marginSize, bool drawBorder);
    
    void setPixel (GLubyte* texture, int offset, int size, int x, int y, GLubyte val);

    bool                errorStatus;
    FT_Library          library;

};

#endif
