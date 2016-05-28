//
//  EdgeDetection.cpp
//  Reflection
//
//  Created by macbook on 12/20/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#include "EdgeDetection.h"
#define PI 3.1415926535


int TEXTURE_WIDTH = 256;
int TEXTURE_HEIGHT = 256;
int m_vViewport[4];

unsigned int EdgeDetection::generateTexture(int width,int height,bool isDepth)
{
	unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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

void EdgeDetection::GenerateFBO()
{
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &TEXTURE_WIDTH);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &TEXTURE_HEIGHT);
    
    // create a framebuffer object
    glGenFramebuffers(1, &FboId);
    glBindFramebuffer(GL_FRAMEBUFFER, FboId);
    
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

EdgeDetection::EdgeDetection( Renderer* parent)
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();

    objModel            = new ObjLoader	( RendererHandler );
    textureQuad         = new SimpleTexture( RendererHandler );
}

EdgeDetection::~EdgeDetection()
{
    delete objModel;
    delete textureQuad;
    
    glDeleteFramebuffers(1, &FboId);
    
    glDeleteTextures(1, &textureId);
    textureId = 0;

    glDeleteTextures(1, &depthTextureId);
    depthTextureId = 0;
}

void EdgeDetection::InitModel()
{
    //Initialize the Polka dots
    objModel->InitModel();

    //Initialize the Simple Texture dots
    textureQuad->InitModel();
    
    // Generate the Frame buffer object
    GenerateFBO();
}

void EdgeDetection::EdgeDetect()
{
    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, DefaultFBO);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureId);
    
    program = ProgramManagerObj->Program( ( char * )"EdgeDetection" );
    glUseProgram( program->ProgramID );
    GLint PIXELSIZE = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "pixelSize" );
    glUniform2f(PIXELSIZE, 1.0/TEXTURE_HEIGHT, 1.0/TEXTURE_WIDTH);

    textureQuad->Render();
}

void EdgeDetection::RenderObj()
{
    // Get the default Framebuffer
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &DefaultFBO);
    
    // Bind Framebuffer object
    glBindFramebuffer(GL_FRAMEBUFFER,FboId);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    objModel->Render();
    
    glBindFramebuffer(GL_FRAMEBUFFER, DefaultFBO);
}

void EdgeDetection::SetUpPerspectiveProjection()
{
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    // Setup projection
    TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );
    TransformObj->TransformLoadIdentity();
    TransformObj->TransformSetPerspective(60.0f, 1, 0.01, 100, 0);
    TransformObj->TransformSetMatrixMode( VIEW_MATRIX );
    TransformObj->TransformLoadIdentity();
    TransformObj->TransformTranslate(0.0,0.0,-4.0);
    TransformObj->TransformSetMatrixMode( MODEL_MATRIX );
    TransformObj->TransformLoadIdentity();
    
}

void EdgeDetection::SetUpOrthoProjection()
{
    TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );
    TransformObj->TransformLoadIdentity();
    float span = 1.0;
    TransformObj->TransformOrtho( -span, span, -span, span, -span, span );
    TransformObj->TransformSetMatrixMode( VIEW_MATRIX );
    TransformObj->TransformLoadIdentity();
    
    TransformObj->TransformSetMatrixMode( MODEL_MATRIX );
    TransformObj->TransformLoadIdentity();
}

void EdgeDetection::Render()
{
    SetUpPerspectiveProjection();
    RenderObj();
    
    SetUpOrthoProjection();
    EdgeDetect();
}

void EdgeDetection::TouchEventDown( float x, float y )
{
    objModel->TouchEventDown( x, y );
    textureQuad->TouchEventDown( x, y );
}

void EdgeDetection::TouchEventMove( float x, float y )
{
    textureQuad->TouchEventMove( x, y );
}

void EdgeDetection::TouchEventRelease( float x, float y )
{
    textureQuad->TouchEventRelease( x, y );
}
