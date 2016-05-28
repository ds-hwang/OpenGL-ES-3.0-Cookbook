#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H
#include "GLutils.h"
#include "Texture.h"

class FrameBufferObjectSurface{
public:
    FrameBufferObjectSurface(GLint surfaceWidth = 0, GLint surfaceHeight = 0);
    ~FrameBufferObjectSurface();

    //! Get the frame buffer object ID
    GLuint getFBOID()        { return fboID; }

    //! Get the previous frame buffer object ID
    GLuint previousFBOID()   { return previousFboID; }

    //! Generate the Frame buffer object
    GLuint GenerateFBO();
    
    //! Bind a framebuffer to a framebuffer target
    void Push();

    //! Unbind a framebuffer to a framebuffer target
    void Pop();

    //! Unbind a framebuffer to the default framebuffer
    void PopToDefault();

    //! Get FBO width
    GLint GetWidth() { return width; }
    
    //! Get FBO height
    GLint GetHeight() { return height; }

    //! Get the texture ID of Stencil buffer
    bool SafeBind();
    
    //! Get the texture ID of Stencil buffer
    bool SafeUnbind();

    //! Attach the texture to the FBO
    void AttachTexture(Texture& texture, GLenum attachment = GL_COLOR_ATTACHMENT0, int mipmapLevel = 0);
    
    //! Draw the more than one color buffers
    void DrawBuffer(GLsizei n, const GLenum* bufs);
    
    //! Check the status of the create FBO
    bool CheckFboStatus();
private:
    /**
     Current frame buffer id before generator ID.
     */
    GLint previousFboID;

    /**
     Current frame buffer ID.
     */
    GLuint fboID;

    /**
     Frame buffer object width.
     */
    GLint width;

    /**
     Frame buffer object height.
     */
    GLint height;
};

#endif //FRAME_BUFFER_OBJECT_H
