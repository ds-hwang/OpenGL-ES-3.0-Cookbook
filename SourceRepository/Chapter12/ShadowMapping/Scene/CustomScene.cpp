#include "CustomScene.h"
#include "ObjLoader.h"

CustomScene::CustomScene(std::string name, Object* parentObj):Scene(name, parentObj)
{
    //Create the Frame buffer object
    fbo = new FrameBufferObjectSurface();
    
    // Generate the FBO ID
    fbo->GenerateFBO();
    
    depthTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_DEPTH_COMPONENT32F, GL_FLOAT, GL_DEPTH_COMPONENT,0,true,0,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST );

    // Attach the Depth Buffer to FBO's depth attachment
    fbo->AttachTexture(depthTexture, GL_DEPTH_ATTACHMENT);
    
    // Check the status of the FBO
    fbo->CheckFboStatus();
    
    lightPerspective    = NULL;
    viewersPerspective  = NULL;
}

void CustomScene::initializeScene()
{
    // Create camera view from lights perspective in order to capture depth buffer.
    lightPerspective = new Camera("lightPerspective", this);
    lightPerspective->SetClearBitFieldMask(GL_DEPTH_BUFFER_BIT);
    lightPerspective->SetPosition(glm::vec3(this->lights.at(0)->position));
    lightPerspective->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(lightPerspective);
    
    // Create scene's camera view.
    viewersPerspective = new Camera("Camera1", this);
    viewersPerspective->SetClearBitFieldMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    viewersPerspective->SetPosition(glm::vec3 (25.00000, 25.0,25.00000));
    viewersPerspective->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(viewersPerspective);
    Scene::initializeScene();
}

CustomScene::~CustomScene(void)
{
}

void CustomScene::resize( int w, int h )
{
    lightPerspective->Viewport(0, 0, w, h);
    viewersPerspective->Viewport(0, 0, w, h);
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
    lightPerspective->Render();
    
    // Enable the back face culling.
    glEnable(GL_CULL_FACE);                 // Important to avoid culling artefacts
    glCullFace(GL_FRONT);

    // Enable the polygon offset filling, in order to avoid the artefact on the object backside faces.
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.5f, 20.0f);
    
    for( int i=0; i<models.size();  i++ ){
        currentModel = models.at(i);
        if(!currentModel){
            continue;
        }
        
        // Set LIGHT PASS (PASS ONE) to True, this will avoid fragment
        // shader execution and only records depth buffer.
        ((ObjLoader*)currentModel)->SetLightPass(true);
        currentModel->Render();
    }
    // Reset to previous frame buffer
    fbo->Pop();

    // Bind the texture unit 0 to depth texture of FBO
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture.getTextureID());

    
    // View the scene from camera
    viewersPerspective->Render();
    
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
        
        // PASS TWO => Normal scene rendering
        // Set LIGHT PASS to False, render the scene using fragment shader.
        ((ObjLoader*)currentModel)->SetLightPass(!true);
        
        currentModel->Render();
    }
    return;
}

Camera* CustomScene::GetLightPerspective()
{
    return lightPerspective;
}

