//
//  Circle.h
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

class Circle : public Model
{
    
public:
    // Constructor for Button
    Circle( Scene* parent, Model* model, ModelType type);

    // Destructor for ObjLoader
    virtual ~Circle();

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
    char SCREEN_COORD_X;
    char INNER_RADIUS;
    char OUTER_RADIUS;
    char PAINT_COLOR;

    glm::vec2 texCoordinates[4];
    glm::vec3 vertices[4];
};
#endif // __SIMPLE_TEXTURE__
 
