//
//  BDFishEye.h
//  Reflection
//
//  Created by macbook on 12/20/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __BDFishEye__
#define __BDFishEye__
#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include "ObjLoader.h"

#include <iostream>
class BDFishEye : public Model
{
private:
    void InitModel();
    ObjLoader* objModel;

public:
    BDFishEye( Renderer* parent = 0);
    ~BDFishEye();
    
    void Render();
    // ProjectionSystem
    void SetUpPrespectiveProjection();
    
    // TouchEvents
    void TouchEventDown( float x, float y );
    void TouchEventMove( float x, float y );
    void TouchEventRelease( float x, float y );
};

#endif /* defined(__BDFishEye__) */
