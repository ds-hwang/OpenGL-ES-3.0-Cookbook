#include "CustomModel.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"

// Namespace used
using std::ifstream;
using std::ostringstream;
using namespace glm;
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
CustomModel::CustomModel( Scene* parent, Model* model, ModelType type):Model(parent, model, type)
{
	if (!parent)
		return;
    
	SceneHandler        = parent;
    
    plane =  new ObjLoader	( parent, this, CUBE, None  );
    plane->SetMaterial(Material(MaterialSilver));
    plane->SetName(std::string("Cube"));
    plane->ScaleLocal(25.0, 0.10, 25.0);
    
    sphere =  new ObjLoader	( parent, plane, SPHERE, None );
    sphere->SetMaterial(Material(MaterialGold));
    sphere->SetName(std::string("Sphere"));
    sphere->Translate(7.0, 4.0, 0.0);
    sphere->SetCenter(glm::vec3(10.0, 2.0, 0.0));
    sphere->ScaleLocal(2.0, 2.0, 2.0);
    
    torus =  new ObjLoader	( parent, plane, TORUS, None );
    torus->SetMaterial(Material(MaterialCopper));
    torus->SetName(std::string("Torus"));
    torus->Translate(0.0, 4.0, 0.0);
    torus->ScaleLocal(2.0, 2.0, 2.0);
    
    suzzane =  new ObjLoader	( parent, plane, SUZZANE, None );
    suzzane->SetMaterial(Material(MaterialPewter));
    suzzane->SetName(std::string("Suzzane"));
    suzzane->Translate(-7.0, 4.0, 0.0);
    suzzane->ScaleLocal(2.0, 2.0, 2.0);
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
CustomModel::~CustomModel()
{
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void CustomModel::InitModel()
{

    Model::InitModel();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and cache program

	\param[in] None.
	\return None

*/
void CustomModel::Render()
{
    sphere->Rotate(-2, 1, 0, 0);
    torus->Rotate(-2, 1, 0, 0);
    suzzane->Rotate(2, 0, 1, 0);
    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible)
    {
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void CustomModel::TouchEventDown( float x, float y )
{
    Model::TouchEventDown(x,y);
}

void CustomModel::TouchEventMove( float x, float y )
{
    Model::TouchEventMove(x,y);
}

void CustomModel::TouchEventRelease( float x, float y )
{
    Model::TouchEventRelease(x,y);
}

//// Discuss in There More section.
//
//bool CustomModel::IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint)
//{
//    Model::IntersectWithRay(ray0, intersectionPoint);
//    return false;
//}

