//
//  EdgeDetection.h
//  Reflection
//
//  Created by macbook on 12/20/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __EdgeDetection__
#define __EdgeDetection__
#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "SimpleTexture.h"

#include <iostream>
class EdgeDetection : public Model
{
private:
    void InitModel();
    ObjLoader* objModel;
    SimpleTexture* textureQuad;
    GLuint textureId, depthTextureId;
    int DefaultFBO;
    
    GLuint FboId;
public:
    EdgeDetection( Renderer* parent = 0);
    ~EdgeDetection();
    
    // Framebuffer related functions.
    unsigned int generateTexture(int width,int height,bool isDepth=false);
    void GenerateFBO();
    
    // Render
    void Render();
    void RenderObj();
    void EdgeDetect();
    
    // ProjectionSystem
    void SetUpPerspectiveProjection();
    void SetUpOrthoProjection();
    
    // TouchEvents
    void TouchEventDown( float x, float y );
    void TouchEventMove( float x, float y );
    void TouchEventRelease( float x, float y );
    
};

#endif /* defined(__EdgeDetection__) */
