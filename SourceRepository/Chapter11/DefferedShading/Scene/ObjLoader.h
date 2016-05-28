#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "ModelEx.h"
#include "glutils.h"
#include "RendererEx.h"
#include <string>
#include "WaveFrontOBJ.h"
#include "Light.h"

using namespace std;

enum MeshType{
    SEMI_HOLLOW_CYLINDER = 0,
    SPHERE,
    TORUS,
    SUZZANE,
    ISO_SPHERE,
    CONE,
    CUBE
};

class ObjLoader : public Model
{
    
public:
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );

public:
    // Constructor for ObjLoader
    ObjLoader( Scene* parent, Model* model, MeshType mesh, ModelType type);

    // Destructor for ObjLoader
    virtual ~ObjLoader();

    // Initialize our Model class
    void InitModel();

    // Render the Model class
    void Render();

    // Apply scenes light on the object
    void ApplyLight();

    // Apply material on the object
    void ApplyMaterial();

    bool IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint);
    
    // Set the light pass
    void SetLightPass(bool flag);
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
    
    // Vertex Array Object ID
    GLuint OBJ_VAO_Id;

    // VBO Stride
    int stride;
    
    // Vertex offset
    GLvoid* offset;

    // Vertex Texture offset in interleaved array.
    GLvoid* offsetTexCoord;
    char MVP, MV, M, SHADOW_MAT, SHADOW_TEX;
    char IS_LIGHT_PASS;
    GLint NormalMatrix;
    int ModelNumber = 0;
    GLuint vertexBuffer;
    bool isPicked;
    bool isLightPass;
    
    // Material variables
    char MaterialAmbient, MaterialSpecular, MaterialDiffuse, MaterialShininess;
    
    // Light variables
    char LightAmbient,LightSpecular, LightDiffuse, LightPosition;
    char Picking;
    glm::mat4 lightBiasMat;
};


#endif // OBJLOADER_H
