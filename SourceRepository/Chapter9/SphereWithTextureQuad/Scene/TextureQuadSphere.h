#ifndef TEXTURE_QUAD_SPHERE_H
#define TEXTURE_QUAD_SPHERE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class TextureQuadSphere : public Model
{
private:
    GLuint positionAttribHandle;
    GLuint colorAttribHandle;
    GLuint radianAngle;
    
    float degree;

public:
    TextureQuadSphere( Renderer* parent = 0);
    TextureQuadSphere();

    void InitModel();
    void Render();
    void RenderSphere();
    char MVP;
    char SCREEN_COORD_X;
    char SCREEN_COORD_Y;
    char SCREEN_W;
    char SCREEN_H;
    
    // Touch Events
    void TouchEventDown( float x, float y );
    void TouchEventMove( float x, float y );
    void TouchEventRelease( float x, float y );

};

#endif // TEXTURE_QUAD_SPHERE_H
