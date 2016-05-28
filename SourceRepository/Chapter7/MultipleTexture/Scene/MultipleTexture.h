//
//  MultipleTexture.h
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __MultipleTexture__
#define __MultipleTexture__

#include <iostream>

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

#include <string>
#include "PngImage.h"

using namespace std;
using namespace namespaceimage;

class MultipleTexture : public Model
{
    
public:
    // Constructor for ObjLoader
    MultipleTexture( Renderer* parent = 0);
    
    // Destructor for ObjLoader
    ~MultipleTexture();

    // Initialize our Model class
    void InitModel();
    
    // Render the Model class
    void Render();
    
    // Touch Events
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
    void loadMultiTexture();
    char* creatPath(char* fname, char* fileName);
private:
    char MVP;
    char TEX;
    char TEX2;
    Image* image;
    Image* image2;
};

#endif // OBJLOADER_H
