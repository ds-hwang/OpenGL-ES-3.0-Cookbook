#ifndef TEAPOT_H
#define TEAPOT_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Teapot : public Model
{
private:
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
    void RenderCubeVBO();
public:
    Teapot( Renderer* parent = 0);
    ~Teapot();

    void InitModel();
    void Render();
    
};

#endif // TEAPOT_H
