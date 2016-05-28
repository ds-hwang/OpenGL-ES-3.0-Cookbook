#pragma once
#include "Scene.h"
//#include "Event.h"
#include <vector>

//class Event
//{
//public:
//    Event(){}
//    ~Event(){}
//
//    //! Gesture Press Event
//    virtual void TouchEventDown( float a, float b ) = 0;
//    
//    //! Gesture Move Event
//    virtual void TouchEventMove( float a, float b ) = 0;
//    
//    //! Gesture Release Event
//    virtual void TouchEventRelease( float a, float b ) = 0;
//};

class Renderer : public Object
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

//    //! Gesture Press Event
//    void TouchEventDown( float a, float b );
//    
//    //! Gesture Move Event
//    void TouchEventMove( float a, float b );
//    
//    //! Gesture Release Event
//    void TouchEventRelease( float a, float b );

    //! Cache the scene
    void addScene( Scene* scene);

    //! Remove the scene
    bool removeScene( Scene* scene);

    //! Get Scene by Name
    //Scene* getScene(char* name);
};
