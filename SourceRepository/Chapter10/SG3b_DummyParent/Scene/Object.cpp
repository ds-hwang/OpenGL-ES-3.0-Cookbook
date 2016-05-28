#include "Object.h"

Object::Object(std::string objectName, Object* parentObj)
{
    parent  = NULL; // Important: do not remove this check, the SetParent method checks for parent's Null pointer condition.
    name    = objectName;
    SetParent(parentObj);
    return;
}

Object::~Object()
{
}

void Object::SetParent(Object* parentModel)
{
    // If there existing a parent
    if (this->parent != NULL){
        RemoveParent();
    }
    
    parent = parentModel;
    if(parent){
        parent->SetChild(this);
    }
}

void Object::RemoveParent()
{
    RemoveFromParentChildList();
    parent = NULL;
}

void Object::SetChild(Object* child)
{
    for(int i =0; i<childList.size(); i++){
        if(child == childList.at(i)){
            return;
        }
    }
    child->parent = this;
    childList.push_back(child);
}

void Object::RemoveFromParentChildList()
{
    for(int i =0; parent && i<parent->childList.size(); i++){
        if(this == parent->childList.at(i)){
            parent->childList.erase (parent->childList.begin()+i);
            return;
        }
    }
}

Object* Object::GetParent(){
    return parent;
}

std::vector<Object*>* Object::GetChildren()
{
    return &childList;
}
