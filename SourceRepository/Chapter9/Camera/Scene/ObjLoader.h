#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include <string>
class Vertex
{
public:
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
    Vertex()
    {
        position.x = position.y = position.z = 0.0f;
        uv.x = uv.y = 0.0f;
        normal.x = normal.y = normal.z = 0.0f;
    }
};

class FaceIndex
{
public:
    short vertexIndex;
    short normalIndex;
    short uvIndex;
    FaceIndex(short v, short u, short n)
    {
        vertexIndex = v;
        uvIndex     = u;
        normalIndex = n;
    }
};

#define MAX_FILE_NAME 1000
struct Mesh
{
    // Obj File name
    char fileName[MAX_FILE_NAME];

    // List of Face Indices For vertex, uvs, normal
	std::vector<FaceIndex> vecFaceIndex;
    
    // List of vertices containing interleaved information for position, uv, and normals
	std::vector<Vertex>    vertices;

    // List of vertices containing positions
	std::vector<glm::vec3> positions;

    // List of vertices containing normal
	std::vector<glm::vec3> normals;
    
    // List of vertices containing uvs
	std::vector<glm::vec2> uvs;
    
    // List of face indices
	std::vector<unsigned short> indices;

    Mesh()
    {
        memset(fileName, 0, MAX_FILE_NAME);
    }
};


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

    // Parse the wavefront OBJ mesh
    bool PareObjMesh(char* path);
    
private:
    Mesh objMeshModel;
};

#endif // OBJLOADER_H
