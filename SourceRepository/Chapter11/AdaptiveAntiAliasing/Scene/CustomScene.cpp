#include "CustomScene.h"
#include "ObjLoader.h"

CustomScene::CustomScene(std::string name, Object* parentObj):Scene(name, parentObj)
{
    camera              = NULL;
}

void CustomScene::initializeScene()
{
    // Create scene's camera view.
    camera = new Camera("Camera1", this);
    camera->SetClearBitFieldMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->SetPosition(glm::vec3 (10.00000, 10.0, 10.00000));
    camera->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(camera);
    Scene::initializeScene();
}

CustomScene::~CustomScene(void)
{
}

void CustomScene::resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void CustomScene::render()
{
    // View the scene from camera
    camera->Render();
    
    // Cull the back face of the objects.
    glCullFace(GL_BACK);
    
    // Disable the polygon offset filling
    glDisable(GL_POLYGON_OFFSET_FILL);
    
    // Render the models
    for( int i=0; i<models.size();  i++ ){
        currentModel = models.at(i);
        if(!currentModel){
            continue;
        }
        
        currentModel->Render();
    }
    return;
}
