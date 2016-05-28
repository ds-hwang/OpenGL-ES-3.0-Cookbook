#include "ModelEx.h"
#include "Scene.h"

bool Model::useProgram(char* programName) 
{
	return false;
}
Model::Model(Scene*	handler, Model* model, ModelType type, std::string objectName):Object(objectName, model) //Parminder Obj Rem
{
    SceneHandler        = handler;
    ProgramManagerObj	= handler->SceneProgramManager();
    TransformObj		= handler->SceneTransform();
    modelType           = type;
    transformation      = glm::mat4();
    transformationLocal = glm::mat4();
    center              = glm::vec3(0.0, 0.0, 0.0);
    isVisible           = true;
}

Model::~Model()
{
    RemoveParent();
    RemoveFromParentChildList();
}

/*!
 Load textures, initialize shaders, etc.
 */
void Model::InitModel()
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->InitModel();
    }
}

void Model::Rotate( float angle, float x, float y, float z )
{
    transformation = glm::translate( transformation, center);
    transformation = glm::rotate( transformation, angle, glm::vec3( x, y, z ) );
    transformation = glm::translate( transformation, -center);
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

glm::mat4 Model::GetEyeCoordinatesFromRoot()
{
    return  GetParentTransformation(dynamic_cast<Model*>(parent)) * transformation * transformationLocal;
}

glm::mat4 Model::GetParentTransformation(Model* parent)
{
    if(parent){
        return  GetParentTransformation(dynamic_cast<Model*>(parent->parent)) * parent->transformation;
    }
    return glm::mat4(); // return identity matrix.
}

void Model::Render()
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->Render();
    }
}

bool Model::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
{
    
}

void Model::TouchEventDown( float x, float y )
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->TouchEventDown( x, y );
    }
}

void Model::TouchEventMove( float x, float y )
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->TouchEventMove( x, y );
    }
}

void Model::TouchEventRelease( float x, float y )
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->TouchEventRelease( x, y );
    }
}

void Model::setSceneHandler(Scene* sceneHandle){
    SceneHandler = sceneHandle;
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->setSceneHandler( sceneHandle );
    }
}

void Model::SetVisible(bool flag, bool applyToChildren)
{
    isVisible = flag;
    
    if(applyToChildren){
        for(int i =0; i<childList.size(); i++){
            dynamic_cast<Model*>(childList.at(i))->SetVisible( flag, applyToChildren );
        }
    }
}

void Model::ApplyModelsParentsTransformation()
{
    *TransformObj->TransformGetModelMatrix() = *TransformObj->TransformGetModelMatrix()*transformation;
}

void Model::ApplyModelsLocalTransformation()
{
    *TransformObj->TransformGetModelMatrix() = *TransformObj->TransformGetModelMatrix()*transformationLocal;    
}

/*!
    DummyModel CTOR.
 */
DummyModel::DummyModel(Scene*	parentScene, Model* model, ModelType type,std::string objectName):Model(parentScene, model, type, objectName)
{
}

/*!
    DummyModel DTOR.
 */
DummyModel::~DummyModel()
{
    
}

/*!
    The render method does not render anything but applies the preserved relative transformation.
    The local transformation here does not make sense since the object cannot be rendered and additionally the local transformation is also never carried on the children.
 */
void DummyModel::Render()
{
    
    SceneHandler->SceneTransform()->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    // DUMMY RENDERING
    
    Model::Render();
    SceneHandler->SceneTransform()->TransformPopMatrix();  // Parent Child Level
}

