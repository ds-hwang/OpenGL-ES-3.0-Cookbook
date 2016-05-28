//
//  SimpleTexture.h
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __CustomModel__
#define __CustomModel__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "RendererEx.h"

#include <string>
#include "PngImage.h"
#include "Event.h"
#include "ObjLoader.h"

using namespace std;
using namespace namespaceimage;

class CustomModel : public Model
{
    
public:
    // Constructor for CustomModel
    CustomModel( Scene* parent, Model* model, ModelType type);

    // Destructor for CustomModel
    virtual ~CustomModel();

    // Initialize our Model class
    void InitModel();
    
    // Render the Model class
    void Render();
    
    // Touch Events
	void TouchEventDown( float x, float y );
    
	void TouchEventMove( float x, float y );
	
    void TouchEventRelease( float x, float y );

    //bool IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint);

private:
    char MVP;
    char TEX;
    Image* image;
    glm::vec2 texCoordinates[4];
    glm::vec3 vertices[4];
    char* textureImage;
    
    ObjLoader* plane;
    ObjLoader* sphere;
    ObjLoader* torus;
    ObjLoader* suzzane;
};
#endif // CustomModel_H
 
