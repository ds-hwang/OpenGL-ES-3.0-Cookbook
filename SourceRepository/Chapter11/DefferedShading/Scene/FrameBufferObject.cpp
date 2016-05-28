#include "FrameBufferObject.h"
#include "Texture.h"

FrameBufferObjectSurface::FrameBufferObjectSurface(GLint surfaceWidth, GLint surfaceHeight)
{
    previousFboID   =   0;
    fboID           =   0;
    
    if(surfaceWidth == 0){
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    }
    else{
        width = surfaceWidth;
    }
    
    if(surfaceHeight == 0){
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    }
    else{
        height = surfaceHeight;
    }
    
    //bitwiseTexMask  = texturBufferMask;
}

FrameBufferObjectSurface::~FrameBufferObjectSurface()
{
    glDeleteFramebuffers(1, &fboID);
    previousFboID   =   0;
    fboID           =   0;    
}

// Bind a framebuffer to a framebuffer target
void FrameBufferObjectSurface::Push(){
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

// Unbind a framebuffer to a framebuffer target
void FrameBufferObjectSurface::Pop()
{
    glBindFramebuffer(GL_FRAMEBUFFER, previousFboID);
}

// Unbind a framebuffer to the default framebuffer
void FrameBufferObjectSurface::PopToDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObjectSurface::GenerateFBO()
{
    // Get current Framebuffer
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFboID);

    // create a framebuffer object
    glGenFramebuffers(1, &fboID);
//    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
//    glBindFramebuffer(GL_FRAMEBUFFER, previousFboID);
    return fboID;
}

bool FrameBufferObjectSurface::CheckFboStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
#if DEBUG
        printf("Framebuffer creation fails: %d", status);
#endif
        return false;
    }
    return true;
}

void FrameBufferObjectSurface::AttachTexture(Texture& texture, GLenum attachment, int mipmapLevel)
{
    SafeBind();
    glFramebufferTexture2D( GL_FRAMEBUFFER, attachment,
                           texture.getTargetType(), texture.getTextureID(), mipmapLevel );
    SafeUnbind();
}

void FrameBufferObjectSurface::DrawBuffer(GLsizei n, const GLenum* bufs)
{
    SafeBind();
    glDrawBuffers(n, bufs);
    SafeUnbind();
}


bool FrameBufferObjectSurface::SafeBind()
{
    // If the current FBO ID is different from the Frame buffer Id only then perform bind
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &previousFboID );
    
    if (fboID != (GLuint)previousFboID){
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
        return true;
    }

    return false;
}

bool FrameBufferObjectSurface::SafeUnbind()
{
    // Returns id binding to the previously enabled fbo
    if (fboID != (GLuint)previousFboID){
        glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)previousFboID);
        return true;
    }
    
    return false;
}
