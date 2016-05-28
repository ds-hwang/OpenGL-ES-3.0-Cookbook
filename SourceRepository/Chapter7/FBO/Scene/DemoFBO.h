//
//  DemoFBO.h
//  Reflection
//
//  Created by macbook on 1/25/14.
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
class DemoFBO : public Model
{
private:
    void InitModel();
    ObjLoader* objModel;
    SimpleTexture* textureQuad;
    GLuint fboId, rboId, textureId, depthTextureId;
    
public:
    DemoFBO( Renderer* parent = 0);
    ~DemoFBO();
    unsigned int generateTexture(int width,int height,bool isDepth=false);
    void GenerateFBO();
    void GenerateFBOWithRenderBuffer();
    void Render();
    void TouchEventDown( float x, float y );
    void TouchEventMove( float x, float y );
    void TouchEventRelease( float x, float y );
};

#endif /* defined(__Reflection__DemoFBO__) */
