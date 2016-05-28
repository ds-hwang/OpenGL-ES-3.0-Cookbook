#include "CustomScene.h"
#include "ObjLoader.h"

CustomScene::CustomScene(std::string name, Object* parentObj):Scene(name, parentObj)
{
    //Create the Frame buffer object
    fbo = new FrameBufferObjectSurface();
    
    // Generate the FBO ID
    fbo->GenerateFBO();
    
    colorTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_RGB16F, GL_FLOAT, GL_RGB, 0, true, 0, 0, GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

    // Attach the Color Buffer to FBO's color attachment 0
    fbo->AttachTexture(colorTexture, GL_COLOR_ATTACHMENT0);
    
    // Check the status of the FBO
    fbo->CheckFboStatus();
    
    lightPrespective    = NULL;
    camera              = NULL;
}

void CustomScene::initializeScene()
{
    // Create camera view from lights perspective in order to capture depth buffer.
    lightPrespective = new Camera("lightPrespective", this);
    lightPrespective->SetClearBitFieldMask(GL_DEPTH_BUFFER_BIT);
    lightPrespective->SetPosition(glm::vec3(this->lights.at(0)->position));
    lightPrespective->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(lightPrespective);
    
    // Create scene's camera view.
    camera = new Camera("Camera1", this);
    camera->SetClearBitFieldMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->SetPosition(glm::vec3 (25.00000, 25.0,25.00000));
    camera->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(camera);
    Scene::initializeScene();
}

CustomScene::~CustomScene(void)
{
}

void CustomScene::resize( int w, int h )
{
    lightPrespective->Viewport(0, 0, w, h);
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
    // Set Framebuffer to the FBO
    fbo->Push();
    
    // Render the scene from lights perspective
    lightPrespective->Render();
    
    // Enable the back face culling.
    glEnable(GL_CULL_FACE);                 // Important to avoid culling artefacts
    glCullFace(GL_FRONT);

    // Enable the polygon offset filling, in order to avoid the artefact on the object backside faces.
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(12.5f, 25.0f);
    
    for( int i=0; i<models.size();  i++ ){
        currentModel = models.at(i);
        if(!currentModel){
            continue;
        }
        
        // Set LIGHT PASS to True, this will avoid fragment shader execution and only records depth buffer.
        ((ObjLoader*)currentModel)->SetLightPass(true);
        currentModel->Render();
    }
    // Reset to previous frame buffer
    fbo->Pop();

    // Bind the texture unit 0 to depth texture of FBO
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture.getTextureID());

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
        
        // Set LIGHT PASS to False, render the scene using fragment shader.
        ((ObjLoader*)currentModel)->SetLightPass(!true);
        
        currentModel->Render();
    }
    return;
}

Camera* CustomScene::GetLightPrespective()
{
    return lightPrespective;
}

