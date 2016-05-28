#include "FontShaping.h"

void FontShaping::setDirectionAndScript(hb_buffer_t *&buffer, LanguageType languageType)
{
    switch( languageType )
    {
        case Thai:{
            hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
            hb_buffer_set_script(buffer, HB_SCRIPT_THAI);
        }
            break;
            
        case Punjabi:{
            hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
            hb_buffer_set_script(buffer, HB_SCRIPT_GURMUKHI);
        }
            break;
            
        case Arabic:{
            hb_buffer_set_direction(buffer, HB_DIRECTION_RTL);
            hb_buffer_set_script(buffer, HB_SCRIPT_ARABIC);
        }
            break;
            
        case Tamil:{
            hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
            hb_buffer_set_script(buffer, HB_SCRIPT_TAMIL);
        }
            break;
            
        default:{
            hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
            hb_buffer_set_script(buffer, HB_SCRIPT_COMMON);
        }
            break;
    }
}

bool FontShaping::produceShape(const char* str, std::vector< FT_UInt >& codePoints)
{

    FT_UInt glyph_index = 0;
    
    hb_glyph_info_t *glyph_info;
    FT_Face     ft_face = fontFace;      /* handle to face object */
    if (!ft_face){
        return false;
    }
    
    int num_chars = (int)strlen(str);
    
    
    /* Create a buffer for harfbuzz to use */
    if (buffer){
        hb_buffer_destroy(buffer);
        buffer = NULL;
    }
    buffer = hb_buffer_create();
    
    setDirectionAndScript(buffer, languageType);
    
    /* Layout the text */
    hb_buffer_add_utf8(buffer, str, num_chars, 0, num_chars);
    if (!font){
        font = hb_ft_font_create(ft_face, NULL);
    }
    hb_shape(font, buffer, NULL, 0);
    
    int glyph_count = hb_buffer_get_length(buffer);
    glyph_info  = hb_buffer_get_glyph_infos(buffer, 0);
    
    for (int i = 0; i < glyph_count; i++) {
        glyph_index = glyph_info[i].codepoint;
        codePoints.push_back(glyph_index);
    }

    if (buffer){
        hb_buffer_destroy(buffer);
        buffer = NULL;
    }
    
    // If Code Pointer contains data
    if (codePoints.size() <=0 )
        return false;
    
    return true;
}
