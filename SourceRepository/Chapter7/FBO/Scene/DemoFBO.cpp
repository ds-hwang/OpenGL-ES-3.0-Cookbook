//
//  DemoFBO.cpp
//  Reflection
//
//  Created by macbook on 1/25/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#include "DemoFBO.h"

#define TEXTURE_WIDTH 512
#define TEXTURE_HEIGHT 512
int m_vViewport[4];

unsigned int DemoFBO::generateTexture(int width,int height,bool isDepth)
{
	unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (isDepth){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        
    }
    else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

	int error;
	error=glGetError();
	if(error != 0)
	{
		std::cout << "Error: Fail to generate texture." << error << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D,0);
	return texId;
}

//! This function create the FBO in which it uses the Frame buffer's depth buffer for depth testing.
void DemoFBO::GenerateFBO()
{
    // create a framebuffer object
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    //Create color and depth buffer texture object
    textureId = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT);
    depthTextureId = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT, true);
   
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. Color attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           textureId,             // 4. color texture ID
                           0);                    // 5. mipmap level: 0(base)
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_DEPTH_ATTACHMENT,   // 2. Depth attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           depthTextureId,        // 4. depth texture ID
                           0);                    // 5. mipmap level: 0(base)
    
     // check FBO status
     GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
     if(status != GL_FRAMEBUFFER_COMPLETE){
         printf("Framebuffer creation fails: %d", status);
     }
    
    // Bind to default buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//! This function is an alternate way to create the FBO in which it uses Render Buffer's depth buffer for depth testing.
void DemoFBO::GenerateFBOWithRenderBuffer()
{

    // create a framebuffer object
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    // attach the texture to FBO color attachment point
    textureId = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           textureId,             // 4. tex ID
                           0);                    // 5. mipmap level: 0(base)
    
    // create a renderbuffer object to store depth info
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, TEXTURE_WIDTH,TEXTURE_HEIGHT);

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                              GL_DEPTH_ATTACHMENT, // 2. attachment point
                              GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                              rboId);              // 4. rbo ID
    
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
        printf("Framebuffer creation fails: %d", status);
    }
}

DemoFBO::DemoFBO( Renderer* parent)
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();

    objModel            = new ObjLoader	( RendererHandler );
    textureQuad         = new SimpleTexture( RendererHandler );
}

DemoFBO::~DemoFBO()
{
    delete objModel;
    delete textureQuad;
    
    glDeleteFramebuffers(1, &fboId);
    fboId = 0;
    
    glDeleteRenderbuffers(1, &rboId);
    rboId = 0;
    
    glDeleteTextures(1, &textureId);
    textureId = 0;
}

void DemoFBO::InitModel()
{
    //Initialize the Polka dots 
    objModel->InitModel();

    //Initialize the Simple Texture dots
    textureQuad->InitModel();
    
    // Generate the Frame buffer object
    GenerateFBO();
}

void DemoFBO::Render()
{
    // Render to Texture
    int CurrentFbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fboId);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    objModel->Render();
    glBindFramebuffer(GL_FRAMEBUFFER, CurrentFbo);
    TransformObj->TransformError();
    
    // Render Quad with render buffer mapped.
    glViewport(0, 0, RendererHandler->screenWidthPixel()*2, RendererHandler->screenHeightPixel()*2);
	glClearColor(0.710,0.610,0.30,1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    textureQuad->Render();
    TransformObj->TransformError();
}

void DemoFBO::TouchEventDown( float x, float y )
{
    objModel->TouchEventDown( x, y );
    textureQuad->TouchEventDown( x, y );
}

void DemoFBO::TouchEventMove( float x, float y )
{
    textureQuad->TouchEventMove( x, y );
}

void DemoFBO::TouchEventRelease( float x, float y )
{
    textureQuad->TouchEventRelease( x, y );
}

