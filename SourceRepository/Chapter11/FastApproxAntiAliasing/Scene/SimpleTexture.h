//
//  SimpleTexture.h
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __SIMPLE_TEXTURE__
#define __SIMPLE_TEXTURE__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "RendererEx.h"

using namespace std;

class SimpleTexture : public Model
{
    
public:
    // Constructor for Button
    SimpleTexture( Scene* parent, Model* model, ModelType type, glm::vec3* vertices, glm::vec2* textureCoordinates);

    // Destructor for ObjLoader
    virtual ~SimpleTexture();

    // Initialize our Model class
    void InitModel();
    
    // Render the Model class
    void Render();
    
    // Touch Events
	void TouchEventDown( float x, float y );
    
	void TouchEventMove( float x, float y );
	
    void TouchEventRelease( float x, float y );

private:
    char MVP;
    char TEX;
    char SCREEN_COORD_X;
    char SCREEN_RESOLUTION;

    glm::vec2 texCoordinates[4];
    glm::vec3 vertices[4];
};
#endif // __SIMPLE_TEXTURE__
 
