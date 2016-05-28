#include "WaveFrontOBJ.h"
using namespace std;
using namespace glm;

bool OBJMesh::CalculateNormal(bool flatShading)
{
    // If normal information is missing from the OBJ file then it could be calculated using the position
    // and face index information. It can be either calculated in Flat Shading or Smooth Shading.
    
    if( objMeshModel.normals.size() == 0 ){
        // Boolean flag to control type of shading information required.
        // Smooth Shading: Common vertice among faces share unique normal which is calculated after taking resultant of all the normal among those faces. These are called vertex normals.
        // Flat Shading: All face have there own normal which are basically face normals.
        
        // Make space to store the normal information
        objMeshModel.normals.resize(objMeshModel.positions.size());
        int index0, index1, index2;
        glm::vec3 a, b, c;
        for(int i=0; i<objMeshModel.indices.size();i += 3)
        {
            // Use indices to retrive the vertices
            index0 = objMeshModel.indices.at(i);
            index1 = objMeshModel.indices.at(i+1);
            index2 = objMeshModel.indices.at(i+2);
            
            // Retrive each triangles vertex
            a = objMeshModel.positions.at(index0);
            b = objMeshModel.positions.at(index1);
            c = objMeshModel.positions.at(index2);
            
            // Calculate the normal triangle face.
            glm::vec3 faceNormal = glm::cross((b - a), (c - a));
            
            if ( flatShading ){
                // Calculate normals for flat shading
                objMeshModel.vertices[i].normal += faceNormal;
                objMeshModel.vertices[i+1].normal += faceNormal;
                objMeshModel.vertices[i+2].normal += faceNormal;
            }
            else
            {
                // Calculate normals for smooth shading
                objMeshModel.normals[index0] += faceNormal;
                objMeshModel.normals[index1] += faceNormal;
                objMeshModel.normals[index2] += faceNormal;
            }
        }
        // Store the calculated normals
        if ( !flatShading ){
            for(int i = 0; i < objMeshModel.vecFaceIndex.size(); i++)
            {
                int index = objMeshModel.vecFaceIndex.at(i + 0).vertexIndex;
                objMeshModel.vertices[i].normal = objMeshModel.normals.at(index);
            }
        }
        
        for (int j=0;j<objMeshModel.vertices.size(); j++){
            objMeshModel.vertices[j].normal = glm::normalize(objMeshModel.vertices[j].normal);
        }
    }
    return true;
}

// Mathematics for 3D Game Programming and Computer Graphics, 3rd edition
// http://www.terathon.com/code/tangent.html
bool OBJMesh::CalculateTangents()
{
    vector<vec3> tan1Accum;
    vector<vec3> tan2Accum;

    objMeshModel.tangents.resize(objMeshModel.positions.size());

    for( uint i = 0; i < objMeshModel.positions.size(); i++ ) {
        tan1Accum.push_back(vec3(0.0f));
        tan2Accum.push_back(vec3(0.0f));
        objMeshModel.tangents.push_back(vec4(0.0f));
    }
    
    int index0, index1, index2;
    int index0uv, index1uv, index2uv;

    // Compute the tangent vector
    for( uint i = 0; i < objMeshModel.vecFaceIndex.size(); i += 3 )
    {

        index0 = objMeshModel.vecFaceIndex.at(i).vertexIndex;
        index1 = objMeshModel.vecFaceIndex.at(i+1).vertexIndex;
        index2 = objMeshModel.vecFaceIndex.at(i+2).vertexIndex;

        const vec3 &p0 = objMeshModel.positions.at(index0);
        const vec3 &p1 = objMeshModel.positions.at(index1);
        const vec3 &p2 = objMeshModel.positions.at(index2);

        index0uv = objMeshModel.vecFaceIndex.at(i).uvIndex;
        index1uv = objMeshModel.vecFaceIndex.at(i+1).uvIndex;
        index2uv = objMeshModel.vecFaceIndex.at(i+2).uvIndex;

        const vec2 &tc1 = objMeshModel.uvs.at(index0uv);
        const vec2 &tc2 = objMeshModel.uvs.at(index1uv);
        const vec2 &tc3 = objMeshModel.uvs.at(index2uv);
    
        // Equation 1:
        // Q1 = P1 − P0
        // Q2 = P2 − P0

        vec3 q1 = p1 - p0;
        vec3 q2 = p2 - p0;
        
        // Equation 2:
        // (s1, t1) = (u1 − u0, v1 − v0)
        // (s2, t2) = (u2 − u0, v2 − v0)
        
        // Equation 3:
        // Q1 = s1T + t1B
        // Q2 = s2T + t2B
        
        float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
        float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
        
        // Equation 4:
        // [ (Q1)x   (Q1)y  (Q1)z ]       [(s1,  t1)] [(Tx   Ty  Tz)]
        //                            =
        // [ (Q2)x   (Q2)y  (Q2)z ]       [(s2,  t2)] [(Bx   By  Bz)]

        // Equation 5:
        // [(Tx   Ty  Tz)]      [      1       ]  [(s1,  -t1)] [ (Q1)x   (Q1)y  (Q1)z ]
        //                  =   ----------------
        // [(Bx   By  Bz)]      [(-s1t2 - s2t1)]  [(-s2,  t2)] [ (Q2)x   (Q2)y  (Q2)z ]

        float r = 1.0f / (s1 * t2 - s2 * t1);
        
        vec3 tan1( (s1*q1.x - t1*q2.x) * r,
                   (s1*q1.y - t1*q2.y) * r,
                   (s1*q1.z - t1*q2.z) * r); // Tagent
        
        vec3 bTan( (t2*q2.x - s2*q1.x) * r,
                   (t2*q2.y - s2*q1.y) * r,
                   (t2*q2.z - s2*q1.z) * r); // BiTagent

        tan1Accum[index0] += tan1;
        tan1Accum[index1] += tan1;
        tan1Accum[index2] += tan1;
        
        tan2Accum[index0] += bTan;
        tan2Accum[index1] += bTan;
        tan2Accum[index2] += bTan;
    }
    
    for( uint i = 0; i < objMeshModel.positions.size(); ++i )
    {
        objMeshModel.tangents[i] = vec4(glm::normalize( tan1Accum[i] ),1.0);
        // It is advise to store the BiTangents into an array also instead of calclating at fly time in vertex shader.
        printf("\nT: %f, %f, %f", objMeshModel.tangents[i].x, objMeshModel.tangents[i].y, objMeshModel.tangents[i].z);
    }

    for(int i = 0; i < objMeshModel.vecFaceIndex.size(); i++)
    {
        int index = objMeshModel.vecFaceIndex.at(i + 0).vertexIndex;
        objMeshModel.vertices[i].tangent = objMeshModel.tangents.at(index);
        printf("\nP: %f, %f, %f", objMeshModel.vertices[i].tangent.x, objMeshModel.vertices[i].tangent.y, objMeshModel.vertices[i].tangent.z);
    }

    tan1Accum.clear();
    tan2Accum.clear();
    
    return true;
}

bool OBJMesh::CreateInterleavedArray()
{
    int faceIdxSize = (int)objMeshModel.vecFaceIndex.size();
        objMeshModel.vertices.resize( faceIdxSize );
        objMeshModel.indices.resize( faceIdxSize );
    
    // Get the total number of indices.
    IndexCount = (int)objMeshModel.indices.size();
    
    //Create the interleaved vertex information for Vertex containing position, uv and normal.
    for(int i = 0; i < faceIdxSize; i++)
    {
        //Position information must be avilable always
        int index = objMeshModel.vecFaceIndex.at(i + 0).vertexIndex;
        objMeshModel.vertices[i].position = objMeshModel.positions.at(index);
        objMeshModel.indices[i] = (GLushort)objMeshModel.vecFaceIndex.at(i).vertexIndex;
        
        // If UV information is avilable.
        if(objMeshModel.uvs.size()){
            index = objMeshModel.vecFaceIndex.at(i).uvIndex;
            objMeshModel.vertices[i].uv = objMeshModel.uvs.at(index);
        }
        
        // If Normal information is avilable.
        if(objMeshModel.normals.size())
        {
            index = objMeshModel.vecFaceIndex.at(i ).normalIndex;
            objMeshModel.vertices[i].normal = objMeshModel.normals.at(index);
        }
    }
    
    if ( faceIdxSize )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool OBJMesh::ScanVertexNormalAndUV( FILE* pFile, char c )
{
    float  x, y, z, u, v;
    switch(c)
    {
        case ' ': // Loading vertices
            fscanf(pFile,"%f %f %f\n",&x,&y,&z);
            objMeshModel.positions.push_back(glm::vec3(x, y, z));
            break;
        case 'n': // Loading normals
            fscanf(pFile,"%f %f %f\n",&x,&y,&z);
            objMeshModel.normals.push_back(glm::vec3(x, y, z));
            break;
        case 't': // Loading UVs
            fscanf(pFile,"%f %f\n",&u,&v);
            objMeshModel.uvs.push_back(glm::vec2(u, v));
            break;
        default:
            return false;
    }
    return true;
}

bool OBJMesh::ScanFaceIndex( FILE* pFile, char c )
{
    int    vIndex, nIndex, uvIndex;
    float  scanCount;

    // If Normal and UV information availble. Format => v/u/n
    if ( objMeshModel.normals.size() && objMeshModel.uvs.size() )
    {
        
        while(fgetc(pFile) != '\n'){
            vIndex = nIndex = uvIndex = 0;
            
            scanCount = fscanf(pFile,"%d/%d/%d",&vIndex,&uvIndex,&nIndex);
            if((scanCount != 3) || (vIndex == 0) || (uvIndex == 0) || (nIndex == 0))
            {
                break;
            }
            
            objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
        }
    }
    
    // If Normal information avialbe and UV missing. Format => v//n
    else if( objMeshModel.normals.size() && !objMeshModel.uvs.size() )
    {
        while(fgetc(pFile) != '\n'){
            vIndex = nIndex = uvIndex = 0;
            scanCount = fscanf(pFile,"%d//%d",&vIndex, &nIndex);
            if((scanCount != 2) || (vIndex == 0) || (nIndex == 0))
            {
                break;
            }
            
            objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
        }
    }
    
    // If Normal information missing and UV avialbe. Format => v/u/
    else if( !objMeshModel.normals.size() && objMeshModel.uvs.size() )
    {
        while(fgetc(pFile) != '\n'){
            vIndex = nIndex = uvIndex = 0;
            scanCount = fscanf(pFile,"%d/%d/",&vIndex, &uvIndex);
            if((scanCount != 2) || (vIndex == 0) || (uvIndex == 0))
            {
                break;
            }
            
            objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
        }
    }
    
    // If both Normal and UV information missing. Format => v
    else if( !objMeshModel.normals.size() && !objMeshModel.uvs.size() )
    {
        vIndex = nIndex = uvIndex = 0;
        while(fgetc(pFile) != '\n'){
            scanCount = fscanf(pFile,"%d",&vIndex);
            if((scanCount != 1) || (vIndex == 0))
            {
                break;
            }
            objMeshModel.vecFaceIndex.push_back(FaceIndex(vIndex-1, uvIndex-1, nIndex-1));
        }
        
    }
    return true;
}


bool OBJMesh::ParseFileInfo(char* path)
{
    FILE*  pFile;
    bool   eofReached = false;
    char   c;
    
    pFile = fopen ( path,"r" );
    if( pFile==NULL )
    {
        LOGI("\nFail to open Obj mesh file: %s", path);
        fclose (pFile);
        return false;
    }
    
    strcpy(objMeshModel.fileName, path);
    while(!eofReached)
    {
        c = fgetc(pFile);
        switch(c)
        {
            case '#': // Comment. Ignore
            case 'u': // Ignore
            case 's': // Ignore
            case 'g': // Grouping not supported
                while(fgetc(pFile) != '\n'); // Skip till new next line not reached.
                break;
                
#ifdef __IPHONE_4_0
			case EOF:
#else
			case (unsigned char)EOF:
#endif
                eofReached = true;
                break;
                
            case 'v': // Loading the vertices.
                c = fgetc(pFile); // The next character will let us know what vertex attribute to load
                ScanVertexNormalAndUV( pFile, c );
                break;
                
            case 'f':// 'f' means it is a face index information in the form of v/u/n
                ScanFaceIndex( pFile, c );
                break;
        }
    }
    fclose (pFile);
    return true;
}

Mesh* OBJMesh::ParseObjModel(char* path, bool flatShading)
{
	// Parse’s the obj file
    if(!ParseFileInfo(path)){
        return NULL;
    }

    // Interleaved data array
	CreateInterleavedArray();
    
    // Generate the normals
	CalculateNormal( flatShading );

    // Generate tangents
	if(objMeshModel.uvs.size()){
        CalculateTangents();
    }
    ClearMesh();
    
    return &objMeshModel;
}

bool OBJMesh::ClearMesh()
{
    // Deallocate position coordinates
    objMeshModel.positions.clear();

	// Deallocate normals coordinates
    objMeshModel.normals.clear();

	// Deallocate tex Coords
    objMeshModel.uvs.clear();

	// Deallocate normals coordinates
    objMeshModel.tangents.clear();

	// Deallocate indices
    objMeshModel.indices.clear();

    // Vertex Face indices
	objMeshModel.vecFaceIndex.clear();
    return true;
}
