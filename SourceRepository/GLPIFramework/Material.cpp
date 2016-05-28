#include "Material.h"

Material::Material(glm::vec4  ambient, glm::vec4 diffuse, glm::vec4 specular, GLfloat shiness)
{
    ambient     = ambient;
    diffuse     = diffuse;
    specular    = specular;
    shiness     = shiness;
}

Material::Material(const Material& mat)
{
    ambient     = glm::vec4 (mat.ambient);
    diffuse     = glm::vec4 (mat.diffuse);
    specular    = glm::vec4 (mat.specular);
    shiness     = mat.shiness;
}

Material& Material::operator = (const Material & mat)
{
    ambient     = glm::vec4 (mat.ambient);
    diffuse     = glm::vec4 (mat.diffuse);
    specular    = glm::vec4 (mat.specular);
    shiness     = mat.shiness;
    return *this;
}

Material::Material(MaterialType type) {
    
    typeOfMaterial = type;
    
    switch(type)
    {
        case MaterialGold:
        {
            ambient     = GoldAmbient;
            diffuse     = GoldDiffuse;
            specular    = GoldSpecular;
            shiness     = GoldShiness;
            break;
        }
        
        case MaterialCopper:
        {
            ambient     = CopperAmbient;
            diffuse     = CopperDiffuse;
            specular    = CopperSpecular;
            shiness     = CopperShiness;
            break;
        }
        
        case MaterialPewter:
        {
            ambient     = PewterAmbient;
            diffuse     = PewterDiffuse;
            specular    = PewterSpecular;
            shiness     = PewterShiness;
            break;
        }
        
        case MaterialPolishedCopper:
        {
            ambient     = PolishedCopperAmbient;
            diffuse     = PolishedCopperDiffuse;
            specular    = PolishedCopperSpecular;
            shiness     = PolishedCopperShiness;
            break;
        }
            
        case MaterialPolishedGold:
        {
            ambient     = PolishedGoldAmbient;
            diffuse     = PolishedGoldDiffuse;
            specular    = PolishedGoldSpecular;
            shiness     = PolishedGoldShiness;
            break;
        }
            
        case MaterialSilver:
        {
            ambient     = SilverAmbient;
            diffuse     = SilverDiffuse;
            specular    = SilverSpecular;
            shiness     = SilverShiness;
            break;
        }
            
        case MaterialWhite:
        {
            ambient     = WhiteAmbient;
            diffuse     = WhiteDiffuse;
            specular    = WhiteSpecular;
            shiness     = WhiteShiness;
            break;
        }
            
        case MaterialNone:
        default:
        {
            ambient     = glm::vec4();
            diffuse     = glm::vec4();
            specular    = glm::vec4();
            shiness     = 0.0;
        }
    }
}