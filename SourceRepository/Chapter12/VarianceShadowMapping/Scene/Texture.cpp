#include "Texture.h"

Texture::Texture()
{
    texID           = 0;
    depth           = 0;
    width           = 0;
    height          = 0;
    miniFilter      = GL_LINEAR;
    magniFilter     = GL_LINEAR_MIPMAP_LINEAR;
    texWrapModeR    = GL_CLAMP_TO_EDGE;
    texWrapModeS    = GL_CLAMP_TO_EDGE;
    texWrapModeT    = GL_CLAMP_TO_EDGE;
}

Texture::~Texture()
{
    deleteTexture();
}

void Texture::deleteTexture()
{
    if(glIsTexture(texID)){
        glDeleteTextures(1, &texID);
        texID = 0;
    }
}

GLuint Texture::generateTexture2D(GLenum texTarget, GLuint surfaceWidth, GLuint surfaceHeight,
                                GLint texInternalFormat,
                                GLenum type ,
                                GLenum format ,
                                void *data ,
                                bool buildMipMap ,
                                GLint mip_level,
                                GLint border,
                                GLint wrapModeS ,
                                GLint wrapModeT ,
                                GLint minFilter ,
                                GLint magFilter
                                )
{
    // Make sure if texture is created before then delete it.
    deleteTexture();
    
    internalFormat      = texInternalFormat;
    width               = surfaceWidth;
    height              = surfaceHeight;
    target              = texTarget;
    miniFilter          = minFilter;
    magniFilter         = magFilter;
    texWrapModeS        = wrapModeS;
    texWrapModeT        = wrapModeT;
    
    glGenTextures(1, &texID);
    /*
    // Check the glError here
    glErr = glGetError();
    if(glErr != GL_NO_ERROR){
       return;
    }
     */
    // Allocate OpenGL texture memory and bind to it.
    glBindTexture (target, texID);
    
    // Set texture's S wrapping mode type
    glTexParameteri(target, GL_TEXTURE_WRAP_S, texWrapModeS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, texWrapModeT);
    
    // Set texture's interpolation
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, miniFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magniFilter);
    
    // Set texture's format
    glTexImage2D (target,
                  mip_level,
                  internalFormat,
                  width,
                  height,
                  border,
                  format,
                  type,
                  data);
    
    if(buildMipMap){
        glGenerateMipmap(target);
    }
    
    return texID;
}


GLuint Texture::generateTexture3D(GLenum texTarget, GLuint surfaceWidth, GLuint surfaceHeight,
                                GLuint surfaceDepth,
                                GLint texInternalFormat,
                                GLenum type ,
                                GLenum format ,
                                void *data ,
                                bool buildMipMap ,
                                GLint mip_level,
                                GLint border,
                                GLint wrapModeR ,
                                GLint wrapModeS ,
                                GLint wrapModeT ,
                                GLint minFilter ,
                                GLint magFilter
                                )
{
    // Make sure if texture is created before then delete it.
    deleteTexture();
    
    internalFormat      = texInternalFormat;
    width               = surfaceWidth;
    height              = surfaceHeight;
    depth               = surfaceDepth;
    target              = texTarget;
    miniFilter          = minFilter;
    magniFilter         = magFilter;
    texWrapModeR        = wrapModeR;
    texWrapModeS        = wrapModeS;
    texWrapModeT        = wrapModeT;
    
    glGenTextures(1, &texID);
    /*
     // Check the glError here
     glErr = glGetError();
     if(glErr != GL_NO_ERROR){
     return;
     }
     */
    // Allocate OpenGL texture memory and bind to it.
    glBindTexture (target, texID);
    
    // Set texture's S wrapping mode type
    glTexParameteri(target, GL_TEXTURE_WRAP_R, texWrapModeR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, texWrapModeS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, texWrapModeT);
    
    // Set texture's interpolation
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, miniFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magniFilter);
    
    // Set texture's format
    glTexImage3D (target,
                  mip_level,
                  internalFormat,
                  width,
                  height,
                  depth,
                  border,
                  format,
                  type,
                  data);
    
    if(buildMipMap){
        glGenerateMipmap(target);
    }
    
    return texID;
}
