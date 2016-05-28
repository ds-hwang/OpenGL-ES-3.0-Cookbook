#include "CustomScene.h"
#include "ObjLoader.h"

CustomScene::CustomScene(std::string name, Object* parentObj):Scene(name, parentObj)
{
    //Create the Frame buffer object
    fbo = new FrameBufferObjectSurface();
    GLint w = fbo->GetWidth();
    GLint h = fbo->GetHeight();
    // Generate the FBO ID
    fbo->GenerateFBO();
    
    // Create the color buffer and Depth buffer
    glActiveTexture(GL_TEXTURE0);   // Use texture unit 0
    colorTexture.generateTexture2D(GL_TEXTURE_2D, w, h, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);

    glActiveTexture(GL_TEXTURE1);   // Use texture unit 1
    positionTexture.generateTexture2D(GL_TEXTURE_2D, w, h, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    
    glActiveTexture(GL_TEXTURE2);   // Use texture unit 2
    normalTexture.generateTexture2D(GL_TEXTURE_2D, w, h, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    
    glActiveTexture(GL_TEXTURE3);   // Use texture unit 3
    depthTexture.generateTexture2D(GL_TEXTURE_2D, w, h, GL_DEPTH_COMPONENT32F, GL_FLOAT, GL_DEPTH_COMPONENT );
    
    
    // Attach the Color Buffer to FBO's color attachment 0
    fbo->AttachTexture(depthTexture,    GL_DEPTH_ATTACHMENT);
    fbo->AttachTexture(positionTexture, GL_COLOR_ATTACHMENT1);
    fbo->AttachTexture(normalTexture,   GL_COLOR_ATTACHMENT2);
    fbo->AttachTexture(colorTexture,    GL_COLOR_ATTACHMENT3);

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    fbo->DrawBuffer(4, drawBuffers);
    
    // Check the status of the FBO
    fbo->CheckFboStatus();
    
    lightPerspective    = NULL;
    camera              = NULL;
}

void CustomScene::initializeScene()
{
    // Create camera view from lights perspective in order to capture depth buffer.
    lightPerspective = new Camera("lightPerspective", this);
    //lightPerspective->SetClearBitFieldMask(GL_DEPTH_BUFFER_BIT);
    lightPerspective->SetPosition(glm::vec3(this->lights.at(0)->position));
    lightPerspective->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(lightPerspective);
    
    // Create scene's camera view.
    camera = new Camera("Camera1", this);
    camera->SetClearBitFieldMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->SetPosition(glm::vec3 (50.00000, 50.0,50.00000));
    camera->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    this->addCamera(camera);
    Scene::initializeScene();
}

CustomScene::~CustomScene(void)
{
}

void CustomScene::resize( int w, int h )
{
    lightPerspective->Viewport(0, 0, w, h);
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
    lightPerspective->Render();
    // Enable the back face culling.
    //glEnable(GL_CULL_FACE);                 // Important to avoid culling artefacts
    //glCullFace(GL_FRONT);

    // Enable the polygon offset filling, in order to avoid the artefact on the object backside faces.
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.5f, 20.0f);
    
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
    
    lightPerspective->Clear();
    return;
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

Camera* CustomScene::GetLightPerspective()
{
    return lightPerspective;
}

