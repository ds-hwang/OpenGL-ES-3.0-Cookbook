//
//  CompressedTextureure.h
//  Reflection
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef COMPRESSED_TEXTURE_H
#define COMPRESSED_TEXTURE_H

#include <iostream>

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

#include <string>
#include "PngImage.h"
#include "CompressedImage.h"

using namespace std;
using namespace namespaceimage;

class CompressedTexture : public Model
{
    
public:
    // Constructor for ObjLoader
    CompressedTexture( Renderer* parent = 0);
    
    // Destructor for ObjLoader
    ~CompressedTexture();

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
    CompressedImage* compressImage;
    
};

#endif // COMPRESSED_TEXTURE_H
