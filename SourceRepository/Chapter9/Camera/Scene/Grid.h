#ifndef GRID_H
#define GRID_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Grid : public Model
{
private:

public:
    Grid( Renderer* parent = 0);
    ~Grid();

    void CreateGrid(GLfloat XDim, GLfloat ZDim, int XDiv, int ZDiv);
    void InitModel();
    void Render();
    void TouchEventDown( float x, float y );
};

#endif // GRID_H
