#ifndef SCENEBASIC_H
#define SCENEBASIC_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Triangle : public Model
{
private:
    GLuint positionAttribHandle;
    GLuint colorAttribHandle;
    GLuint radianAngle;
    
    float degree;
    float radian;

public:
    Triangle( Renderer* parent = 0);
    ~Triangle();

    void InitModel();
    void Render();
};

#endif // SCENEBASIC_H
