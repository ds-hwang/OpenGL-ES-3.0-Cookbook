#ifndef Material_h
#define Material_h

#include "glutils.h"
#include <string>

typedef enum {
    MaterialNone,
    MaterialGold,
    MaterialCopper,
    MaterialPolishedCopper,
    MaterialPolishedGold,
    MaterialSilver,
    MaterialPewter,
    MaterialWhite
} MaterialType;

// Copper Material
const glm::vec4 CopperAmbient(0.19125, 0.0735, 0.0225, 1.0);
const glm::vec4 CopperDiffuse(0.7038, 0.27048, 0.0828, 1.0);
const glm::vec4 CopperSpecular(0.256777, 0.137622, 0.086014, 1.0);
const GLfloat   CopperShiness = 2.8;

// Polished Copper Material
const glm::vec4 PolishedCopperAmbient(0.2295, 0.08825, 0.0275, 1.0);
const glm::vec4 PolishedCopperDiffuse(0.5508, 0.2118, 0.066, 1.0);
const glm::vec4 PolishedCopperSpecular(0.580594, 0.223257, 0.0695701, 1.0);
const GLfloat   PolishedCopperShiness = 51.2;

// Gold Material
const glm::vec4 GoldAmbient(0.24725, 0.1995, 0.0745, 1.0);
const glm::vec4 GoldDiffuse(0.75164, 0.60648, 0.22648, 1.0);
const glm::vec4 GoldSpecular(0.628281, 0.555802, 0.366065, 1.0);
const GLfloat   GoldShiness=51.2;

// Polished Gold Material
const glm::vec4 PolishedGoldAmbient(0.24725, 0.2245, 0.0645, 1.0);
const glm::vec4 PolishedGoldDiffuse(0.34615, 0.3143, 0.0903, 1.0);
const glm::vec4 PolishedGoldSpecular(0.797357, 0.723991, 0.208006, 1.0);
const GLfloat   PolishedGoldShiness = 83.2;

//Silver Material
const glm::vec4 SilverAmbient(0.19225, 0.19225, 0.19225, 1.0);
const glm::vec4 SilverDiffuse(0.50754, 0.50754, 0.50754, 1.0);
const glm::vec4 SilverSpecular(0.508273, 0.508273, 0.508273, 1.0);
const GLfloat   SilverShiness = 51.2;

//Petwer Material
const glm::vec4 PewterAmbient(0.105882, 0.058824, 0.113725, 1.0);
const glm::vec4 PewterDiffuse(0.427451, 0.470588, 0.541176, 1.0);
const glm::vec4 PewterSpecular(0.333333, 0.333333, 0.521569, 1.0);
const GLfloat   PewterShiness = 9.84615;

//Pure White
const glm::vec4 WhiteAmbient(1.0,1.0,1.0,1.0);
const glm::vec4 WhiteDiffuse(1.0,1.0,1.0,1.0);
const glm::vec4 WhiteSpecular(1.0,1.0,1.0,1.0);
const GLfloat   WhiteShiness = 10.0;

class Material
{
public:
    glm::vec4 ambient, diffuse, specular;
    GLfloat shiness;
    std::string name;
    
    MaterialType typeOfMaterial;
    
    Material(glm::vec4  ambient, glm::vec4 diffuse, glm::vec4 specular, GLfloat shiness);
    
    Material(const Material & p);
    
    Material & operator = (const Material & p);
    
    Material(MaterialType type = MaterialNone);
};

#endif
