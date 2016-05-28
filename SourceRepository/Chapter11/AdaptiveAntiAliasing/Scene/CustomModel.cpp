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
    sphere =  new ObjLoader	( parent, this, SPHERE, None );
    sphere->SetMaterial(Material(MaterialGold));
    sphere->SetName(std::string("Sphere"));
    
    sphere->Rotate(30, 0.0, 1.0, 0.0);
    sphere->SetCenter(glm::vec3(10.0, 2.0, 0.0));
    sphere->ScaleLocal(4.0, 4.0, 4.0);
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