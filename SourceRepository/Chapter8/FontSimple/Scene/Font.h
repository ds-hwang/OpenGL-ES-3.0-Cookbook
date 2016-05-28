#ifndef _FONT_H
#define _FONT_H

#include "Model.h"
#include "FontGenerator.h"

class Font : public Model, public FontGenerator {

public:
    ~Font();
	void Render();
    void InitModel();
    Font(const char* ttfFile, int Size, Renderer* parent, LanguageType Language= English);
    void printText (const char* str, GLfloat Red = 1.0f, GLfloat Green = 1.0f, GLfloat Blue = 1.0f, GLfloat Alpha = 1.0f);
    void animateText(const char* str, GLfloat Red = 1.0f, GLfloat Green = 1.0f, GLfloat Blue = 1.0f, GLfloat Alpha = 1.0f,float radius = 5.0f,float rot = 0.0);
    
private:
	void drawGlyph (const Glyph& gi);
    char MVP, TEX, FRAG_COLOR;
};

#endif
