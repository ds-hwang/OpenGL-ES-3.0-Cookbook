#ifndef CUBE_H
#define CUBE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Cube : public Model
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;
	Renderer* RenderObj;

public:
    Cube( Renderer* parent = 0);
    ~Cube();

    void InitModel();
    //void update( float t );
    void Render();
    //void renderRoad();
    //void resize(int, int);
};

#endif // CUBE_H
