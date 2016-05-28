#pragma once
#include "Scene.h"
#include "Event.h"
#include <vector>


class Renderer : public Object, public GestureEvent
{
    //! Scene List
    std::vector <Scene*> scenes;
    
public:
    //! Renderer Constructor
    Renderer();

    //! Renderer Destructor
    ~Renderer();

    //! Initialize Engine
    void initializeScenes();

    //! resize screen
    void resize( int w, int h );

    //! Render the Models
    void render();

    //! Gesture Press Event
    void TouchEventDown( float a, float b );
    
    //! Gesture Move Event
    void TouchEventMove( float a, float b );
    
    //! Gesture Release Event
    void TouchEventRelease( float a, float b );

    //! Cache the scene
    void addScene( Scene* scene);

    //! Remove the scene
    bool removeScene( Scene* scene);

    //! Get Scene by Name
    Scene* getScene(char* name);
};
