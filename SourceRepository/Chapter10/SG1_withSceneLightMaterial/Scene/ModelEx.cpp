#include "ModelEx.h"

bool Model::useProgram(char* programName) 
{
	return false;
}
Model::Model(Scene*	handler, Model* model, ModelType type, std::string objectName)//:Object(objectName, model) //Parminder Obj Rem
{
    SceneHandler        = handler;
    modelType           = type;
//    transformationLocal = glm::mat4();
//    center              = glm::vec3(0.0, 0.0, 0.0);
}

Model::~Model()
{
    //RemoveParent();//Parminder Obj Rem
    //RemoveFromParentChildList();//Parminder Obj Rem
}

/*!
 Load textures, initialize shaders, etc.
 */
void Model::InitModel()
{
//Parminder Obj Rem
//    for(int i =0; i<childList.size(); i++){
//        dynamic_cast<Model*>(childList.at(i))->InitModel();
//    }
}

//void Model::Rotate( float angle, float x, float y, float z )
//{
//    transformation = glm::translate( transformation, center);
//    transformation = glm::rotate( transformation, angle, glm::vec3( x, y, z ) );
//    transformation = glm::translate( transformation, -center);
//}
//
//void Model::Translate(float x, float y, float z )
//{
//    transformation = glm::translate( transformation, glm::vec3( x, y, z ));
//}
//
//void Model::Scale(float x, float y, float z )
//{
//    transformation = glm::scale(transformation, glm::vec3( x, y, z ));
//}
//
//void Model::RotateLocal( float angle, float x, float y, float z )
//{
//    transformationLocal = glm::rotate( transformationLocal, angle, glm::vec3( x, y, z ) );
//}
//
//void Model::TranslateLocal(float x, float y, float z )
//{
//    transformationLocal = glm::translate( transformationLocal, glm::vec3( x, y, z ));
//}
//
//void Model::ScaleLocal(float x, float y, float z )
//{
//    transformationLocal = glm::scale(transformationLocal, glm::vec3( x, y, z ));
//}

//glm::mat4 Model::GetEyeCoordinatesFromRoot()
//{
    //Parminder Obj Rem
//    return GetParentTransformation(dynamic_cast<Model*>(parent)) * transformation * transformationLocal;
//}

//glm::mat4 Model::GetParentTransformation(Model* parent)
//{
    //Parminder Obj Rem
//    if(parent){
//        return parent->transformation * GetParentTransformation(dynamic_cast<Model*>(parent->parent));
//    }
//    return glm::mat4(); // return identity matrix.
//}

void Model::Render()
{
    //Parminder Obj Rem
//    for(int i =0; i<childList.size(); i++){
//        dynamic_cast<Model*>(childList.at(i))->Render();
//    }
}

//bool Model::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
//{
//    
//}

//void Model::TouchEventDown( float x, float y )
//{
//    //Parminder Obj Rem
////    for(int i =0; i<childList.size(); i++){
////        dynamic_cast<Model*>(childList.at(i))->TouchEventDown( x, y );
////    }
//}
//
//void Model::TouchEventMove( float x, float y )
//{
//    //Parminder Obj Rem
////    for(int i =0; i<childList.size(); i++){
////        dynamic_cast<Model*>(childList.at(i))->TouchEventMove( x, y );
////    }
//}
//
//void Model::TouchEventRelease( float x, float y )
//{
//    //Parminder Obj Rem
////    for(int i =0; i<childList.size(); i++){
////        dynamic_cast<Model*>(childList.at(i))->TouchEventRelease( x, y );
////    }
//}

void Model::setSceneHandler(Scene* sceneHandle){
    SceneHandler = sceneHandle;
//    for(int i =0; i<childList.size(); i++){
//        dynamic_cast<Model*>(childList.at(i))->setSceneHandler( sceneHandle );
//    }
}
