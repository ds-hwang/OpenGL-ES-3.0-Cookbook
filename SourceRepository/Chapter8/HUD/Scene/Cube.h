#ifndef CUBE_H
#define CUBE_H

#include "Model.h"
#include "glutils.h"
#include "Renderer.h"

class Cube : public Model
{
private:
	char MVP;
    char attribVertex;
    char attribColor;
    std::vector<glm::vec2> screenCoordinateVector;

public:
    Cube( Renderer* parent = 0);
    ~Cube();

    void InitModel();
    
    void Render();
    void GetScreenCoordinates();
    void TouchEventDown( float x, float y );
    std::vector<glm::vec2>* getScreenCoordinateVertices(){return &screenCoordinateVector;}
    
};

#endif // CUBE_H
