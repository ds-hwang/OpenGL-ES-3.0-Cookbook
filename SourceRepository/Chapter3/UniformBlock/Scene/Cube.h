#ifndef CUBE_H
#define CUBE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Cube : public Model
{
private:
    int width, height;
    GLuint UBO;
	Renderer* RenderObj;

public:
    Cube( Renderer* parent = 0);
    ~Cube();

    void InitModel();
    
    void Render();
    void CreateUniformBufferObject();
    void RenderCube();
};

#endif // CUBE_H
