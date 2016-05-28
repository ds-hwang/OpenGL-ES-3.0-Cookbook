#pragma once
#include "Image.h"

namespace namespaceimage {
    
    struct ETC2Header {
        char name[4];                   // "PKM "
        char version[2];                // "20" for ETC2
        unsigned short format;          // Format == number of mips (== zero)
        unsigned short paddedWidth;     // Padded Texture width dimensions, multiple of 4 (big-endian)
        unsigned short paddedHeight;    // Padded Texture height dimensions, multiple of 4 (big-endian)
        unsigned short origWidth;       // Original width dimensions (big-endian)
        unsigned short origHeight;      // Original height dimensions (big-endian)
    };
    
    class CompressedImage : public Image
    {
        unsigned int byte;
        unsigned int format;
    public:
        CompressedImage(void);
        ~CompressedImage(void);
        
        //! Load the compressed file image.
        virtual bool loadImage(char* fileName, bool generateTexID = true, GLenum target = GL_TEXTURE_2D);
    };

}