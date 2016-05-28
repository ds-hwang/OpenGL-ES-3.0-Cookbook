#ifndef CUBE_H
#define CUBE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Cube : public Model
{
private:
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
    void RenderCubeVBO();
    char MVP;
    char attribVertex;
    char attribColor;
public:
    Cube( Renderer* parent = 0);
    ~Cube();

    void InitModel();
    void Render();
    
};

#endif // CUBE_H
