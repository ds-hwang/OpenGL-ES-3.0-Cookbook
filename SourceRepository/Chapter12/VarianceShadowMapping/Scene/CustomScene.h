#ifndef CUSTOM_SCENE_H
#define CUSTOM_SCENE_H
#include "Scene.h"
#include "FrameBufferObject.h"

class CustomScene : public Scene
{
    
public:
    //! Default Constructor
    CustomScene(std::string name = "", Object* parentObj = NULL);
    
    //! Destructor
    virtual ~CustomScene(void);
    //! Initialize Scene
    void initializeScene();

    void render();
    void resize( int w, int h );
    Camera* GetLightPrespective();
    
private:
    /**
     FBO Scene object.
     */
    FrameBufferObjectSurface* fbo;

    /**
     Color texture buffer.
     */
    Texture colorTexture;

    /**
     Depth texture buffer.
     */
    //Texture depthTexture;
    
    /**
     Camera view from light's perspective.
     */
    Camera* lightPrespective;
    
    /**
     Actual camera view.
     */
    Camera* camera;
};

#endif
