#pragma once

class Event
{
public:
    Event();
    virtual ~Event();
};

class GestureEvent : public Event
{
public:
    GestureEvent();
    virtual ~GestureEvent();
    
    //! Gesture Press Event
    virtual void TouchEventDown( float a, float b ) = 0;
    
    //! Gesture Move Event
    virtual void TouchEventMove( float a, float b ) = 0;
    
    //! Gesture Release Event
    virtual void TouchEventRelease( float a, float b ) = 0;
};