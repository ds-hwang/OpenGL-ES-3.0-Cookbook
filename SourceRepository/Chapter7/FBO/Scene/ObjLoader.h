#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include <string>
#include "../../../GLPIFramework/WaveFrontOBJ/WaveFrontOBJ.h"

using namespace std;
class ObjLoader : public Model
{

public:
    // Constructor for ObjLoader
    ObjLoader( Renderer* parent = 0);

    // Destructor for ObjLoader
    ~ObjLoader();

    // Initialize our Model class
    void InitModel();

    // Render the Model class
    void Render();

    // Touch Events
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
    

private:
    // Load the mesh model
    void LoadMesh();
    
    // Switch the mesh model
    void SwitchMesh();
    
    // Release the mesh model resources
    void ReleaseMeshResources();
    
    // Object for wavefront model parser
    OBJMesh waveFrontObjectModel;

    // Mesh structure object for storing parsed information.
    Mesh* objMeshModel;
    
    // Parse the wavefront OBJ mesh
    unsigned char RenderPrimitive;
    
    // Number of vertex Indices
    int IndexCount;    
};

#endif // OBJLOADER_H
