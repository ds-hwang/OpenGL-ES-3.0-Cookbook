#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Primitives : public Model
{
private:
    unsigned int primitive;
    char mvp;
    char attribVertex;
    char attribColor;
    
public:
    Primitives( Renderer* parent = 0);
    Primitives();

    void InitModel();
    //void update( float t );
    void Render();
    void RenderPrimitives();
    
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
};

#endif // PRIMITIVES_H
