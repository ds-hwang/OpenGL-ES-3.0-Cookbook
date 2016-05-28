#ifndef Ray_h
#define Ray_h

#include "glutils.h"

class Ray
{
public:
    glm::vec3 destination;
    glm::vec3 direction;// destination, dirrection
    Ray();
    Ray(glm::vec3 de, glm::vec3 di);
    Ray(const Ray & r);
    
    Ray & operator = (const Ray & r);
    void printInformation() const;
};

#endif
