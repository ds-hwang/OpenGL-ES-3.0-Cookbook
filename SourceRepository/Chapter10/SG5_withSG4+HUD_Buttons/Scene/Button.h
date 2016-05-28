//
//  SimpleTexture.h
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __Button__
#define __Button__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "RendererEx.h"

#include <string>
#include "PngImage.h"
#include "Event.h"

using namespace std;
using namespace namespaceimage;

class Button : public Model
{
    
public:
    // Constructor for Button
    Button( Scene* parent, Model* model, ModelType type, glm::vec3* vertices, glm::vec2* textureCoordinates, char* texture);

    // Destructor for Button
    virtual ~Button();

    // Initialize our Button class
    void InitModel();
    
    // Render the Button class
    void Render();
    
//    // Touch Events
//	void TouchEventDown( float x, float y );
//    
//	void TouchEventMove( float x, float y );
//	
//    void TouchEventRelease( float x, float y );

    //bool IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint);

private:
//    bool clicked;
    char MVP;
    char TEX;
    Image* image;
    glm::vec2 texCoordinates[4];
    glm::vec3 vertices[4];
    char* textureImage;
};
#endif // OBJLOADER_H
 
