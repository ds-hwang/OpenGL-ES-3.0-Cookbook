//
//  GaussianBlur.h
//  Reflection
//
//  Created by macbook on 12/20/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __Reflection__DemoFBO__
#define __Reflection__DemoFBO__
#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "SimpleTexture.h"

#include <iostream>
class GaussianBlur : public Model
{
private:
    void InitModel();
    ObjLoader* objModel;
    SimpleTexture* textureQuad;
    GLuint textureId, textureId2, depthTextureId;
    int CurrentFbo;
    
    GLuint blurFboId1, blurFboId2;
public:
    GaussianBlur( Renderer* parent = 0);
    ~GaussianBlur();
    
    // Framebuffer related functions.
    unsigned int generateTexture(int width,int height,bool isDepth=false);
    void GenerateBlurFBO1();
    void GenerateBlurFBO2();
    //void GenerateFBOWithRenderBuffer();
    
    // Render
    void Render();
    void RenderObj();
    void RenderHorizontalBlur();
    void RenderVerticalBlur();
    
    // ProjectionSystem
    void SetUpPerspectiveProjection();
    void SetUpOrthoProjection();
    
    // TouchEvents
    void TouchEventDown( float x, float y );
    void TouchEventMove( float x, float y );
    void TouchEventRelease( float x, float y );
    
    // Blur functions
    static float GaussianEquation(float val, float sigma);
};

#endif /* defined(__Reflection__DemoFBO__) */
