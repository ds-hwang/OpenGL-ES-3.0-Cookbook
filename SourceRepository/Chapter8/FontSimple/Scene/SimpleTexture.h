//
//  SimpleTexture.h
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __SimpleTexture__
#define __SimpleTexture__

#include <iostream>

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

#include <string>
#include "PngImage.h"

using namespace std;
using namespace namespaceimage;

class SimpleText : public Model
{
    
public:
    // Constructor for ObjLoader
    SimpleText( Renderer* parent = 0);
    
    // Destructor for ObjLoader
    ~SimpleText();

    // Initialize our Model class
    void InitModel();
    
    // Render the Model class
    void Render();
    
    // Touch Events
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
    void TakeScreenshot();
private:
    char MVP;
    char TEX;
    Image* image;
};

#endif // OBJLOADER_H
