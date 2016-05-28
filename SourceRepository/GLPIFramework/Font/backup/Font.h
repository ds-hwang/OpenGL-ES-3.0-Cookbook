#ifndef _FONT_H
#define _FONT_H

#include "Model.h"
#include "FontShaping.h"

class Font : public Model, public FontShaping {

public:
    ~Font();
	void Render();
    void InitModel();
    Font(char* ttfFile, int Size, Renderer* parent, LanguageType Language= English);
    void printText (char* str, GLfloat Red = 1.0f, GLfloat Green = 1.0f, GLfloat Blue = 1.0f, GLfloat Alpha = 1.0f);

private:
	void drawGlyph (const Glyph& gi);
    char MVP, TEX, FRAG_COLOR;
};

#endif
