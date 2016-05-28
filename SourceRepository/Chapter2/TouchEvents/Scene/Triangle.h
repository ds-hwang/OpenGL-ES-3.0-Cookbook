#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Triangle : public Model
{
private:
    GLuint positionAttribHandle;
    GLuint colorAttribHandle;
	GLfloat  colors[9];
public:
    Triangle( Renderer* parent = 0);
    ~Triangle();

    void InitModel();
    //void update( float t );
    void Render();

	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
};

#endif // TRIANGLE_H
