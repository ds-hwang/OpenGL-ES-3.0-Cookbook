#include "CustomScene.h"
#include "ObjLoader.h"

CustomScene::CustomScene(std::string name, Object* parentObj):Scene(name, parentObj)
{
    //Create the Frame buffer object
    fbo = new FrameBufferObjectSurface();
    
    // Generate the FBO ID
    fbo->GenerateFBO();
    
    // Create the color buffer and Depth buffer
    colorTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    depthTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_DEPTH_COMPONENT32F, GL_FLOAT, GL_DEPTH_COMPONENT );
    
    // Attache the Color Buffer to FBO's color attachment 0
    fbo->AttachTexture(colorTexture, GL_COLOR_ATTACHMENT0);

    // Attache the Depth Buffer to FBO's depth attachment
    fbo->AttachTexture(depthTexture, GL_DEPTH_ATTACHMENT);
    
    // Check the status of the FBO
    fbo->CheckFboStatus();
    
    camera              = NULL;
}

void CustomScene::initializeScene()
{
    // Create scene's camera view.
    camera = new Camera("Camera1", this);
    camera->SetClearBitFieldMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->SetPosition(glm::vec3 (25.0, 25.0, 25.0));
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
    // Set Framebuffer to the FBO
    fbo->Push();
    
    // Render the scene from lights prespective
    camera->Render();
    
    for( int i=0; i<models.size();  i++ ){
        currentModel = models.at(i);
        if(!currentModel){
            continue;
        }
        currentModel->Render();
    }
    // Reset to previous frame buffer
    fbo->Pop();

    // Bind the texture unit 0 to depth texture of FBO
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture.getTextureID());

    camera->Clear();
    return;
}