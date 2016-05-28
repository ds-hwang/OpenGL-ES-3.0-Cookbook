#ifndef OBJECT_H
#define OBJECT_H

#include "constant.h"
#include <string>
#include <vector>
#include <typeinfo>

// Parminder: Interface for the rendering models.
class Object
{
public:
    Object(std::string name = "", Object* parentObj = NULL);
    
    virtual ~Object();

    void SetName(std::string mdlName){ name = mdlName;}
    
    std::string GetName(){return name;}

    void SetParent(Object* parent = 0);
    
    void RemoveParent();
    
    void SetChild(Object* child = 0);
    
    void RemoveFromParentChildList();
    
    Object*  GetParent();
    
    std::vector<Object*>* GetChildren();

protected:

    /*!
     Model's name
     */
    std::string name;

    /*!
     Model's parent
     */
    Object* parent;
    
    /*!
     Model's child list
     */
    std::vector<Object*> childList;
};

#endif // OBJECT_H
