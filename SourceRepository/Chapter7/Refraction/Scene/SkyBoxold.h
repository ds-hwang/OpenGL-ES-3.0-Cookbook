#ifndef CUBE_H
#define CUBE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include "Image.h"

class Skybox : public Model
{
private:
    int width, height;
    namespaceimage::Image* image;
    unsigned char* matter;

public:
    Skybox( Renderer* parent = 0);
    ~Skybox();

    void InitModel();
    
    void Render();
    void RenderCube(bool useVBO = true, bool useDrawElement = true);
    void loadCubeMap();
    char* creatPath(char* fname, char* fileName);
    char MVP;
    char M;

};

#endif // CUBE_H
