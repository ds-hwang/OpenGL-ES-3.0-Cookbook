#include "Ray.h"

Ray::Ray()
{
    destination = glm::vec3();
    direction   = glm::vec3();
}

Ray::Ray(glm::vec3 de, glm::vec3 di)
{
    destination = de;
    direction   = di;
}

Ray::Ray(const Ray & r)
{
    destination = r.destination;
    direction   = r.direction;
}

Ray& Ray::operator = (const Ray & r)
{
    destination = r.destination;
    direction   = r.direction;
    return *this;
}

void Ray::printInformation() const
{
}
