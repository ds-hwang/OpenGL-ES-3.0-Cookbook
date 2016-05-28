//
//  BDFishEye.cpp
//  Reflection
//
//  Created by macbook on 12/20/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#include "BDFishEye.h"

BDFishEye::BDFishEye( Renderer* parent)
{
	if (!parent)
		return;
    
	RendererHandler	= parent;
	ProgramManagerObj	= parent->RendererProgramManager();
	TransformObj		= parent->RendererTransform();

    objModel            = new ObjLoader	( RendererHandler );
}

BDFishEye::~BDFishEye()
{
    delete objModel;
}

void BDFishEye::InitModel()
{
    //Initialize the Polka dots
    objModel->InitModel();
}

void BDFishEye::SetUpPerspectiveProjection()
{
    //glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
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

void BDFishEye::Render()
{
    SetUpPerspectiveProjection();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    objModel->Render();
}

void BDFishEye::TouchEventDown( float x, float y )
{
    objModel->TouchEventDown( x, y );
}

void BDFishEye::TouchEventMove( float x, float y )
{
}

void BDFishEye::TouchEventRelease( float x, float y )
{
}
