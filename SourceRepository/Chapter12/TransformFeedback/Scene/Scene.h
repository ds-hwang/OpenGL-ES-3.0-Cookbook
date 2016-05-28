#ifndef SCENE_H
#define SCENE_H
#include "ModelEx.h"
#include "ProgramManager.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Event.h"
#include <vector>

class Renderer;

class Scene : public Object, public GestureEvent
{
    
public:
    //! Default Constructor
    Scene(std::string name = "", Object* parentObj = NULL);
    
    //! Destructor
    virtual ~Scene(void);
    
    //! Initialize Scene
    void initializeScene();
    
    //! Return the ProgramManager handler
    inline ProgramManager* SceneProgramManager(){ return &ProgramManagerObj; }
    
    //! Return the Transform handler
    inline Transform*  SceneTransform() { return &TransformObj;  }
    
    //! Screen Width returned
    inline int screenWidthPixel() { return screenWidth; }
    
    //! Screen Height returned
    inline int screenHeightPixel() { return screenHeight; }
    
    //! Generate the models
    void createModels();
    
    //! Render the Models
    void render();
    
    //! Initializes the Models
    void initializeModels();
    
    //! Remove all the models
    void clearModels();
    
    //! Initializes the Models
    void setUpModels();
    
    //! Get the model
    Model* getModel( ModelType );
    
    //! Cache the model
    void addModel( Model* );

    //! Add lights
    void addLight( Light* );
    
    //! Get the camera from the scene
    Camera* getCamera( std::string camName );
    
    //! Add the camera to the scene
    void addCamera( Camera* cameraObj );
    
    //! Get the renderer of the scene
    Renderer* getRenderer();
    
    //! Set the renderer of the scene
    void setRenderer( Renderer* renderer);
    
    void removeModel(Model*);
    
    void TouchEventDown( float a, float b );
    
    void TouchEventMove( float a, float b );
    
    void TouchEventRelease( float a, float b );
    
    //! resize screen
    void resize( int w, int h );
    
    void setUpProjection();

    std::vector <Light*>& getLights(){ return lights; }
private:

    //! Program handler for render
    ProgramManager	ProgramManagerObj;
    
    //! Transform operation handler
    Transform	TransformObj;
    
    //! Screen Height
    int screenHeight;
    
    //! Screen Width
    int screenWidth;
    
    //! Model List
    std::vector <Model*> models;

    //! Light List
    std::vector <Light*> lights;

    //! Model List
    std::vector <Camera*> cameras;
    
    
    //! Renderer in which scene is executing
    Renderer* renderManager;

    // Current Camera in use
    Camera* currentCamera;
    
    // Current Model in use
    Model* currentModel;
};

#endif
