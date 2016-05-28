#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include <string>
#include "WaveFrontOBJ.h"

using namespace std;
class ObjLoader : public Model
{
    
private:
    GLuint VertexBuffer;
    int IndexCount;

	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );

public:
    // Constructor for ObjLoader
    ObjLoader( Renderer* parent = 0);
    
    // Destructor for ObjLoader
    ~ObjLoader();

    // Initialize our Model class
    void InitModel();
    
    // Render the Model class
    void Render();

    
private:
    OBJMesh waveFrontObjectModel;
    Mesh* objMeshModel;
    
    glm::vec3 cameraPosition; //#N
};

#endif // OBJLOADER_H
