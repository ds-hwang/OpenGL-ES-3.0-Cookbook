#include "CompressedImage.h"
#include "../GLutils.h"

using namespace namespaceimage;

CompressedImage::CompressedImage(void)
{
	memData.extension = PNG;
}

CompressedImage::~CompressedImage(void)
{
	//LOGI("destructor: PNG");
	if ( memData.bitsraw )
		delete memData.bitsraw;
	memData.bitsraw = NULL;
	if (memData.texID){
		glDeleteTextures( 1,&memData.texID );
		memData.texID	= 0;
	}
}

//! Load the bitmap file helper.
bool CompressedImage::loadImage(char* fileName, bool generateTexID, GLenum target )
{
	//memData.bitsraw = read_png_file( fileName);
    FILE *fp = fopen(fileName, "rb");
    if (!fp){
        return false; //[read_png_file] File could not be opened for reading
    }
    ETC2Header pkmfile;
    
    fread(&pkmfile, sizeof(ETC2Header), 1, fp);
    pkmfile.format          = GLUtils::swap_uint16(pkmfile.format);
    pkmfile.paddedWidth     = GLUtils::swap_uint16(pkmfile.paddedWidth);
    pkmfile.paddedHeight    = GLUtils::swap_uint16(pkmfile.paddedHeight);
    pkmfile.origWidth       = GLUtils::swap_uint16(pkmfile.origWidth);
    pkmfile.origHeight      = GLUtils::swap_uint16(pkmfile.origHeight);
    memData.width           = pkmfile.paddedWidth;
    memData.height          = pkmfile.paddedHeight;
    unsigned int imageSize  = ceil(memData.width/4) * ceil(memData.height/4) * 8;
    memData.bitsraw         = (unsigned char*) malloc(imageSize);
    
    fread(memData.bitsraw, imageSize, 1, fp);
	if (!memData.bitsraw){
		return false;
	}

    if ( generateTexID ){
        GLuint texID;
        glGenTextures	( 1,&texID );
        if (target == GL_TEXTURE_2D){
            glBindTexture	( GL_TEXTURE_2D,texID );
        }
        else{
			#ifdef __ANDROID__
			glBindTexture	( 0x8513/*GL_TEXTURE_CUBE_MAP*/,texID );
			#else
            glBindTexture	( GL_TEXTURE_CUBE_MAP,texID );
			#endif
        }
        memData.texID = texID;
    }
    GLenum format = 0;
    if(pkmfile.version[0] == '2' && pkmfile.version[1] == '0')
    {
        switch (pkmfile.format) {
            case 0:
                // Handle case 1:
                break;
            case 1:
                format = GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
                break;
            case 2:
                // Handle case 2:
                break;
            case 3:
                // Handle case 3:
                break;
                
            default:
                // Handle case default:
                break;
        }
    }
    else{
     //   imgData->format = PF_ETC1_RGB8;
    }
    glCompressedTexImage2D(	GL_TEXTURE_2D, 0,
                           format,
                           memData.width,
                           memData.height,
                           0,
                           imageSize,
                           memData.bitsraw);
    
	return true;
}
