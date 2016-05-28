//
//  Bloom.cpp
//
//  Created by macbook on 12/20/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#include "Bloom.h"
#define PI 3.1415926535
#define NIL -1

int TEXTURE_WIDTH = 256;
int TEXTURE_HEIGHT = 256;
int m_vViewport[4];

unsigned int Bloom::generateTexture(int width,int height,bool isDepth)
{
	unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

void Bloom::GenerateSceneFBO()
{
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &TEXTURE_WIDTH);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &TEXTURE_HEIGHT);

    //TEXTURE_WIDTH /=4.0;
    //TEXTURE_HEIGHT /=4.0;
    
    // create a framebuffer object
    glGenFramebuffers(1, &SceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);
    
    //Create color and depth buffer texture object
    SceneTexture = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT);
    DepthTexture = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT, true);
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. Color attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           SceneTexture,          // 4. color texture ID
                           0);                    // 5. mipmap level: 0(base)
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_DEPTH_ATTACHMENT,   // 2. Depth attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           DepthTexture,          // 4. depth texture ID
                           0);                    // 5. mipmap level: 0(base)
    
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
        printf("Framebuffer creation fails: %d", status);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

void Bloom::GenerateBloomFBO()
{
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &TEXTURE_WIDTH);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &TEXTURE_HEIGHT);

    //TEXTURE_WIDTH /=4.0;
    //TEXTURE_HEIGHT /=4.0;
   
    // create a framebuffer object
    glGenFramebuffers(1, &BloomFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, BloomFbo);
    
    //Create color and depth buffer texture object
    BloomTexture = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT);
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. Color attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           BloomTexture,            // 4. color texture ID
                           0);                    // 5. mipmap level: 0(base)
    
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
        printf("Framebuffer creation fails: %d", status);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::GenerateBlurFBO()
{
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &TEXTURE_WIDTH);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &TEXTURE_HEIGHT);
    
    // create a framebuffer object
    glGenFramebuffers(1, &BlurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, BlurFbo);
    
    //Create color and depth buffer texture object
    BlurTexture = generateTexture(TEXTURE_WIDTH,TEXTURE_HEIGHT);
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                           GL_COLOR_ATTACHMENT0,  // 2. Color attachment point
                           GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                           BlurTexture,          // 4. color texture ID
                           0);                    // 5. mipmap level: 0(base)
    
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE){
        printf("Framebuffer creation fails: %d", status);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Bloom::Bloom( Renderer* parent)
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();

    objModel            = new ObjLoader	( RendererHandler );
    textureQuad         = new SimpleTexture( RendererHandler );
}

Bloom::~Bloom()
{
    delete objModel;
    delete textureQuad;
    
    glDeleteFramebuffers(1, &SceneFbo);
    glDeleteRenderbuffers(1, &BlurFbo);
    
    glDeleteTextures(1, &SceneTexture);
    SceneTexture = 0;

    glDeleteTextures(1, &BloomTexture);
    BloomTexture = 0;

    glDeleteTextures(1, &DepthTexture);
    DepthTexture = 0;
}

void Bloom::InitModel()
{
    //Initialize the Polka dots
    objModel->InitModel();

    //Initialize the Simple Texture dots
    textureQuad->InitModel();
    
    // Generate the Frame buffer object
    GenerateSceneFBO();
    GenerateBloomFBO();
    GenerateBlurFBO();
}

void Bloom::RenderBloom(GLint BindTexture, GLint framebuffer, GLint ColorTexture, GLint DepthBuf)
{
    
    // Bind Framebuffer 1
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture, 0);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, BindTexture);
    
    program = textureQuad->ApplyShader(BloomShader);
    textureQuad->Render();
}

void Bloom::RenderHorizontalBlur(GLint BindTexture, GLint framebuffer, GLint ColorTexture, GLint DepthBuf)
{
    glDisable(GL_DEPTH_TEST);

    // Bind Framebuffer 2
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture, 0);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, BindTexture);
    
    program = textureQuad->ApplyShader(HorizontalBlurShader);
    GLint PIXELSIZE = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "pixelSize" );
    glUniform2f(PIXELSIZE, 1.0/TEXTURE_HEIGHT, 1.0/TEXTURE_WIDTH);
    
    textureQuad->Render();
    
    TransformObj->TransformError();
}

void Bloom::RenderVerticalBlur(GLint BindTexture,GLint framebuffer, GLint ColorTexture, GLint DepthBuf)
{
    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,BindTexture);
    
    program = textureQuad->ApplyShader(VerticalBlurShader);
    GLint PIXELSIZE = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "pixelSize" );
    glUniform2f(PIXELSIZE, 1.0/TEXTURE_HEIGHT, 1.0/TEXTURE_WIDTH);

    textureQuad->Render();
    glActiveTexture (GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
}

void Bloom::RenderObj(GLint BindTexture,GLint framebuffer, GLint ColorTexture, GLint DepthBuf)
{
    
    // Bind Framebuffer 1
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthBuf, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    static float rot = 0.0;
    rot++;
    for(int i=-3;i<3;i++){
        for(int j=-3;j<3;j++){
            TransformObj->TransformPushMatrix();
            TransformObj->TransformTranslate(0.0, 0.0, -5.0);
            TransformObj->TransformRotate(rot , 0.0, 1.0, 0.0);
            TransformObj->TransformTranslate(i*2, -2, j*2);
            TransformObj->TransformScale(0.6, 0.6, 0.6);
            objModel->Render();
            TransformObj->TransformPopMatrix();
        }
    }
    
    //glBindFramebuffer(GL_FRAMEBUFFER, CurrentFbo);
}

void Bloom::SetUpPerspectiveProjection()
{
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    // Setup projection
    TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );
    TransformObj->TransformLoadIdentity();
    GLfloat aspectRatio = (GLfloat)RendererHandler->screenWidthPixel() / (GLfloat)RendererHandler->screenHeightPixel();
    TransformObj->TransformSetPerspective(60.0f, aspectRatio, 0.01, 100, 0);
    TransformObj->TransformSetMatrixMode( VIEW_MATRIX );
    TransformObj->TransformLoadIdentity();
    glm::vec3 eye(0.0, 3.0, 5.0);
    glm::vec3 target(0.0, 0.0, 0.0);
    glm::vec3 up(0.0, 1.0, 0.0);
    TransformObj->TransformLookAt(&eye, &target, &up);

    //TransformObj->TransformTranslate(0.0,0.0,-4.0);
    TransformObj->TransformSetMatrixMode( MODEL_MATRIX );
    TransformObj->TransformLoadIdentity();
    
}

void Bloom::SetUpOrthoProjection()
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

void Bloom::Render()
{
    SetUpPerspectiveProjection();
    
    // Get current Framebuffer
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &DefaultFrameBuffer);

    RenderObj           (NIL,           SceneFbo,   SceneTexture,   DepthTexture);
    
    SetUpOrthoProjection();

    RenderBloom         (SceneTexture,  BloomFbo,   BloomTexture,   NIL);
    RenderHorizontalBlur(BloomTexture,  BlurFbo,    BlurTexture,    NIL);
    RenderVerticalBlur  (BlurTexture,   DefaultFrameBuffer, NIL,            NIL);
}

void Bloom::TouchEventDown( float x, float y )
{
    objModel->TouchEventDown( x, y );
    textureQuad->TouchEventDown( x, y );
}

void Bloom::TouchEventMove( float x, float y )
{
    textureQuad->TouchEventMove( x, y );
}

void Bloom::TouchEventRelease( float x, float y )
{
    //textureQuad->TouchEventRelease( x, y );
}

float Bloom::GaussianEquation(float val, float sigma)
{
    double coefficient = 1.0/(2.0*PI*sigma);
    double exponent    = -(val*val)/(2.0*sigma);
    return (float) (coefficient*exp(exponent));
}
