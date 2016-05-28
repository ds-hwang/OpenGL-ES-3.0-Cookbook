#ifndef WAVEFRONTOBJ_H
#define WAVEFRONTOBJ_H
#include <vector>
#include "GLutils.h"
#define MAX_FILE_NAME 1024

class Vertex
{
public:
	//! Store the vertex position's X, Y and X coordinate
	glm::vec3 position;

	//! Store texture UV coordinates
	glm::vec2 uv;

	//! Store the normal X, Y, Z components
	glm::vec3 normal;

	//! Store the tangent's X, Y, Z, W components
	glm::vec4 tangent;

	//! Vertex class constructor
    Vertex()
    {
        position.x  = position.y = position.z = 0.0f;
        uv.x = uv.y = 0.0f;
        normal.x    = normal.y  = normal.z = 0.0f;
        tangent.x   = tangent.y = tangent.z = 0.0f;
    }
};

class FaceIndex
{
public:
	//! Store the vertex index
    short vertexIndex;

	//! Store the normal index
	short normalIndex;
		
	//! Store the texture index
    short uvIndex;

	//! Constructor of FaceIndex
    FaceIndex(short v, short u, short n)
    {
        vertexIndex = v;
        uvIndex     = u;
        normalIndex = n;
    }
};

struct Mesh
{
    //! Obj File name
    char fileName[MAX_FILE_NAME];
    
    //! List of Face Indices For vertex, uvs, normal
	std::vector<FaceIndex> vecFaceIndex;
    
    //! List of vertices containing interleaved information for position, uv, and normals
	std::vector<Vertex>    vertices;
    
    //! List of vertices containing positions
	std::vector<glm::vec3> positions;
    
    //! List of vertices containing normal
	std::vector<glm::vec3> normals;
    
    //! List of vertices containing uvs
	std::vector<glm::vec2> uvs;
    
    //! List of tangents
	std::vector<glm::vec4> tangents;

    //! List of face indices
	std::vector<unsigned short> indices;
    
	//! Mesh constructors
    Mesh()
    {
        memset(fileName, 0, MAX_FILE_NAME);
    }
};

class OBJMesh
{
    //! Object of Mesh
	Mesh objMeshModel;
    
	//! Used to store the total numver of vertex indices.
	int IndexCount;
    
public:
    OBJMesh(){};
    ~OBJMesh(){};
    
    // Parse the Wavefront OBJ mesh
    Mesh* ParseObjModel(char* path, bool flatShading = false);

	//! Return total number of Indexes
    int IndexTotal(){return  IndexCount;}

private:
	//! Parse’s the obj file
    bool ParseFileInfo(char* path);
    
	//! Parse vertex index, vertex normal and vertex texuture
	bool ScanVertexNormalAndUV( FILE* pFile, char c );
    
	//! Parse the indices information from the face
	bool ScanFaceIndex( FILE* pFile, char c );
    
	//! Generate the normals
	bool CalculateNormal(bool flatShading);
    
	//! Generate the tangents
	bool CalculateTangents();
    
	//! Interleaved data array
	bool CreateInterleavedArray();
    
	//! Release allocated resources
	bool ClearMesh();
};

#endif // WAVEFRONTOBJ_H
