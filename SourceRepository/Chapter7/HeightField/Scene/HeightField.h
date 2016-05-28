#ifndef HeightField_H
#define HeightField_H
#include "Model.h"
#include "Renderer.h"
#include "PngImage.h"
using namespace namespaceimage;

class HeightField: public Model
{

public:
    // Constructor
    HeightField(Renderer* parent, float rDimension, float cDimension, int Rows = 10, int Columns = 10 );

    // Destructor
	~HeightField();
    
    // Initialize our Model class
    void InitModel();
    
    // Render the Model class
    void Render();
    void TouchEventDown( float a, float b );

private:
    // Image object
    Image *imageHeightMap, *imageGrass;
    
    // Image texture dimensions
    int imageHeight, imageWidth;
    
    // uniform attributes locations
    char MVP, FACTOR, TEX_HEIGHT, TEX_GRASS;
    float rot;

    GLint NormalMatrix;
    
    
    GLuint HeightFieldVAO_Id;   // VAO of Height Field
    GLuint vId, iId;            // VBO and IBO
    int faces;                  // Number of faces

    // Size of vertices, texture, faces indexes, color
    int sizeofVertex, sizeofTex, sizeofFace, sizeofColor;
    float *v, *n, *tex;         // temporary buffers
    unsigned short *faceIdx;
};
#endif // HeightField_H

