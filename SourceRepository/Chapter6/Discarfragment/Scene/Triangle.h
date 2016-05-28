#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Triangle : public Model
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;
	Renderer* RenderObj;
	GLfloat  colors[9];
public:
    Triangle( Renderer* parent = 0);
    ~Triangle();

    void InitModel();
    //void update( float t );
    void Render();
    void RenderTriangle();
    void RenderTeapot();

	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
};

#endif // TRIANGLE_H
