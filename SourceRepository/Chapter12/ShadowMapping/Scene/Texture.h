#ifndef TEXTURE_H
#define TEXTURE_H
#include "GLutils.h"

class Texture{
public:
    Texture();
    ~Texture();
    GLuint generateTexture2D(GLenum target, GLuint surfaceWidth, GLuint surfaceHeight,
                           GLint  internalFormat,
                           GLenum type ,
                           GLenum format ,
                           void *data = 0,
                           bool buildMipMap = true,
                           GLint mipMp_level = 0,
                           GLint border = 0,
                           GLint Swrap_mode = GL_CLAMP_TO_EDGE,
                           GLint Twrap_mode = GL_CLAMP_TO_EDGE,
                           GLint min_filter = GL_LINEAR,
                           GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR
                           );
    
    GLuint generateTexture3D(GLenum target, GLuint surfaceWidth, GLuint surfaceHeight,
                           GLuint surfaceDepth,
                           GLint  internalFormat,
                           GLenum type ,
                           GLenum format ,
                           void *data = 0,
                           bool buildMipMap = true,
                           GLint mipMp_level = 0,
                           GLint border = 0,
                           GLint Rwrap_mode = GL_CLAMP_TO_EDGE,
                           GLint Swrap_mode = GL_CLAMP_TO_EDGE,
                           GLint Twrap_mode = GL_CLAMP_TO_EDGE,
                           GLint min_filter = GL_LINEAR,
                           GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR
                           );
    
    GLuint getTextureID() { return texID; }
    
    void deleteTexture();
    
    GLenum getTargetType() { return target; }
private:
    
    /**
     GL ID
     */
    GLuint texID;
    
    /**
     Depth of the Texture
    */
    GLuint depth;

    /** 
     Width of the Texture
    */
    GLuint width;
    
    /** 
     Height  of the Texture 
    */
    GLuint height;
    
    /** 
     texture internal format
    */
    GLint internalFormat;
    
    /**
     target type ( texture 2D)
    */
    GLenum target;
    
    /**
     minification texture filter
    */
    GLint miniFilter;
    
    /** 
     Magnification texture filter
    */
    
    GLint magniFilter;

    /**
     Texture's R wrap mode
     */
    GLint texWrapModeR;
    
    /**
     Texture's S wrap mode
     */
    GLint texWrapModeS;
    
    /**
     Texture's T wrap mode
     */
    GLint texWrapModeT;
};


#endif // TEXTURE_H
