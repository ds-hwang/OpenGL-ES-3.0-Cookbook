#ifndef _FONT_SHAPING_H
#define _FONT_SHAPING_H

#include <hb-ft.h>
#include <vector>
#include "FontGenerator.h"

class FontShaping : public FontGenerator
{
public:
    FontShaping()
    {
        font    = NULL;
        buffer	= NULL;
    }
    ~FontShaping(){}
    void setDirectionAndScript(hb_buffer_t *&buffer, LanguageType languageType);
    bool produceShape(const char* string, std::vector< FT_UInt >&);
    
private:
    hb_font_t *font;
    hb_buffer_t *buffer;
};

#endif
