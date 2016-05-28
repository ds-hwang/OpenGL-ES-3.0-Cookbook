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
    bool produceShape(const char* string, std::vector< FT_UInt >*&, int& glyph_count);
    
private:
    hb_font_t *font;
    hb_buffer_t *buffer;
};

#endif
