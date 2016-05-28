#include "model.h"
#include "Renderer.h"

bool Model::useProgram(char* programName) 
{
	return false;
}
Model::Model(Renderer*	handler, Model* model, ModelType type)
{
    parent      = NULL;
    modelType   = type;
    transformationLocal = glm::mat4();
    SetParent(model);
}

Model::~Model()
{
}

/*!
 Load textures, initialize shaders, etc.
 */
void Model::InitModel()
{
    for(int i =0; i<childList.size(); i++){
        childList.at(i)->InitModel();
    }
}

void Model::Rotate( float angle, float x, float y, float z )
{
    transformation = glm::rotate( transformation, angle, glm::vec3( x, y, z ) );
}

void Model::Translate(float x, float y, float z )
{
    transformation = glm::translate( transformation, glm::vec3( x, y, z ));
}

void Model::Scale(float x, float y, float z )
{
    transformation = glm::scale(transformation, glm::vec3( x, y, z ));
}

void Model::RotateLocal( float angle, float x, float y, float z )
{
    transformationLocal = glm::rotate( transformationLocal, angle, glm::vec3( x, y, z ) );
}

void Model::TranslateLocal(float x, float y, float z )
{
    transformationLocal = glm::translate( transformationLocal, glm::vec3( x, y, z ));
}

void Model::ScaleLocal(float x, float y, float z )
{
    transformationLocal = glm::scale(transformationLocal, glm::vec3( x, y, z ));
}


void Model::SetParent(Model* parentModel)
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

void Model::RemoveParent()
{
    RemoveChild();
    parent = NULL;
}

void Model::SetChild(Model* child)
{
    for(int i =0; i<childList.size(); i++){
        if(child == childList.at(i)){
            printf("Child of type: %d already added into parent of type: %d", child->GetModelType(), GetModelType());
            return;
        }
    }
    child->parent = this;
    childList.push_back(child);
}

void Model::RemoveChild()
{
    for(int i =0; i<parent->childList.size(); i++){
        if(this == parent->childList.at(i)){
            parent->childList.erase (parent->childList.begin()+i);
            printf("Child of type: %d removed from parent of type: %d", this->GetModelType(), GetModelType());
            return;
        }
    }
}

Model* Model::GetParent(){
    return parent;
}

std::vector<Model*>* Model::GetChildren()
{
    return &childList;
}

glm::mat4 Model::GetEyeCoordinatesFromRoot()
{
    return GetParentTransformation(parent) * transformation * transformationLocal;
}

glm::mat4 Model::GetParentTransformation(Model* parent)
{
    if(parent){
        return parent->transformation * GetParentTransformation(parent->parent);
    }
    return glm::mat4(); // return identity matrix.
}

void Model::Render()
{
    for(int i =0; i<childList.size(); i++){
        childList.at(i)->Render();
    }
}

bool Model::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
{
    
}

void Model::TouchEventDown( float a, float b )
{
    for(int i =0; i<childList.size(); i++){
        childList.at(i)->TouchEventDown( a, b );
    }
}
