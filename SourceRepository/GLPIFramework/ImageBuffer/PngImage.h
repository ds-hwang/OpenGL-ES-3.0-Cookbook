#pragma once
#include "Image.h"
#include "../glutils.h"
#include "../libpng/png.h"

namespace namespaceimage {
class PngImage : public Image
{
	unsigned int byte;
	unsigned int format;
    png_byte colorType;    
public:
	PngImage(void);
	~PngImage(void);

	//! Load the bitmap file helper.
	virtual bool loadImage(char* fileName, bool generateTexID = true, GLenum target = GL_TEXTURE_2D);
	unsigned char* read_png_file(char* file_name);//, png_bytep * row_pointers, int* width, int* height);
};

}
