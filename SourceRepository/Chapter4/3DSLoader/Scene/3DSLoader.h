#ifndef _Loader3DS_H
#define _Loader3DS_H

#include <lib3ds/file.h>
#include <lib3ds/chunk.h>
#include "Renderer.h"

struct MyLib3dsUserData
{
	GLint vertexId,indexId;
	GLuint VAOId;
	int vertexSize, normalOffset,indexNum;
	unsigned int strip;
	long landmarkID;
    GLuint textureId;
public:
    MyLib3dsUserData(){
        VAOId = vertexId = indexId = vertexSize = normalOffset = indexNum = strip = landmarkID = textureId = 0;
    }
    
    ~MyLib3dsUserData(){
        VAOId = vertexId = indexId = vertexSize = normalOffset = indexNum = strip = landmarkID = textureId = 0;
    }
};

class Loader3DS : public Model {

  public:
    Loader3DS ( Renderer* parent = 0 );
	~Loader3DS ();

	virtual void Update( float t ) {}

    /*!
      Draw your scene.
      */
	void Render(Lib3dsFile* file);

	/*!
      Draw your 3DS nodes.
      */
	void RenderNodes(Lib3dsFile* file, Lib3dsNode *node = NULL);

	/*!
     Populate the mesh.
     */
    void BuildMesh(	Lib3dsMesh *mesh );

	/*!
     Set the material information into shaders variable.
     */
    void SetMaterialInfo(	Lib3dsMesh *mesh );

    /*!
      Initialize the Landmark by initiating all 3dModel renderer using. For ex: MAX 3DS
      */
	void InitModel();

	/*!
      The method will be called before rendering, this help to set the different states of the model.
      */
	virtual void setStates();

	/*!
      The method will be called after rendering, this help to set the different states of the model.
      */
	virtual void releaseStates();

	/*!
      The method will be called after rendering, this help to set the different states of the model.
      */
	Lib3dsFile* load3dsModel(const char* fileName);

	/*!
      The clear the model, delete cached vertices, indexes, and frame.
      */
	 static bool clearModel(Lib3dsFile* file);

     void Render();


private:
    Lib3dsFile* file;
    unsigned char RenderPrimitive;

	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );    
};

#endif
