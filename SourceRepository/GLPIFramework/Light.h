#ifndef Light_h
#define Light_h

#include "Material.h"

class Light {
private:
    int lightID;
public:
    bool enabled;
    Material material;
    glm::vec4 position;
    GLfloat constantAttenuation, linearAttenuation, quadraticAttenuation;
    Light() {}
    Light(Material mt, glm::vec4 p, GLfloat ca = 1.0, GLfloat la = 0.2, GLfloat qa = 0.05) {
        material = mt;
        position = p;
        constantAttenuation = ca;
        linearAttenuation = la;
        quadraticAttenuation = qa;
        enabled = false;
    }
    Light(const Light & lt) {
        material = Material(lt.material);
        position = glm::vec4(lt.position);
        constantAttenuation = lt.constantAttenuation;
        linearAttenuation = lt.linearAttenuation;
        quadraticAttenuation = lt.quadraticAttenuation;
        enabled = lt.enabled;
    }
    Light & operator = (const Light & lt) {
        material = Material(lt.material);
        position = glm::vec4(lt.position);
        constantAttenuation = lt.constantAttenuation;
        linearAttenuation = lt.linearAttenuation;
        quadraticAttenuation = lt.quadraticAttenuation;
        enabled = lt.enabled;
        return *this;
    }
};

#endif
