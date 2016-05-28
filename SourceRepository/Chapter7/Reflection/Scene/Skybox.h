#ifndef SKYBOX_H
#define SKYBOX_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"
#include "Image.h"

class Skybox : public Model
{
private:
    namespaceimage::Image* image;
    char MVP;

public:
    Skybox( Renderer* parent = 0);
    ~Skybox();

    void InitModel();
    
    void Render();
    void RenderCube(bool useVBO = true, bool useDrawElement = true);
    void createCubeMap();
    char* creatPath(char* fname, char* fileName);
};

#endif // SKYBOX_H
