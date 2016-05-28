//
//  Camera.h
//  ADSPerFragment
//
//  Created by macbook on 2/2/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __ADSPerFragment__Camera__
#define __ADSPerFragment__Camera__

#include <iostream>
#include "glutils.h"
class Renderer;
class Camera
{
public:
    enum CameraBehavior
    {
        CAMERA_BEHAVIOR_FIRST_PERSON,
        CAMERA_BEHAVIOR_FLIGHT
    };
    
    Camera(Renderer* parent = NULL);
    ~Camera();
    
    void lookAt(const glm::vec3 &target);
    void lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);
    void move(float dx, float dy, float dz);
    void move(const glm::vec3 &direction, const glm::vec3 &amount);
    //void perspective(float fovx, float aspect, float znear, float zfar);
    void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);
    void updatePosition(const glm::vec3 &direction, float elapsedTimeSec);
    
    // Getter methods.
    
    const glm::vec3 &getAcceleration() const;
    CameraBehavior getBehavior() const;
    const glm::vec3 &getCurrentVelocity() const;
    const glm::vec3 &getPosition() const;
    const glm::mat4x4 &getProjectionMatrix() const;
    const glm::vec3 &getVelocity() const;
    const glm::vec3 &getViewDirection() const;
    const glm::mat4x4 &getViewMatrix() const;
    const glm::vec3 &getXAxis() const;
    const glm::vec3 &getYAxis() const;
    const glm::vec3 &getZAxis() const;
    
    // Setter methods.
    
    void setAcceleration(float x, float y, float z);
    void setAcceleration(const glm::vec3 &acceleration);
    void setBehavior(CameraBehavior newBehavior);
    void setCurrentVelocity(float x, float y, float z);
    void setCurrentVelocity(const glm::vec3 &currentVelocity);
    void setPosition(float x, float y, float z);
    void setPosition(const glm::vec3 &position);
    void setVelocity(float x, float y, float z);
    void setVelocity(const glm::vec3 &velocity);
    
private:
    void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
    void rotateFirstPerson(float headingDegrees, float pitchDegrees);
    void updateVelocity(const glm::vec3 &direction, float elapsedTimeSec);
    void updateViewMatrix(bool orthogonalizeAxes);
    
    static const float DEFAULT_FOVX;
    static const float DEFAULT_ZFAR;
    static const float DEFAULT_ZNEAR;
    static const glm::vec3 WORLD_XAXIS;
    static const glm::vec3 WORLD_YAXIS;
    static const glm::vec3 WORLD_ZAXIS;
    
    CameraBehavior m_behavior;
    float m_fovx;
    float m_znear;
    float m_zfar;
    float m_aspectRatio;
    float m_accumPitchDegrees;
    glm::vec3 m_eye;
    glm::vec3 m_xAxis;
    glm::vec3 m_yAxis;
    glm::vec3 m_zAxis;
    glm::vec3 m_viewDir;
    glm::vec3 m_acceleration;
    glm::vec3 m_currentVelocity;
    glm::vec3 m_velocity;
    //glm::mat4x4 m_viewMatrix;
    //glm::mat4x4 m_projMatrix;
    Renderer* parent;
};

//-----------------------------------------------------------------------------

inline const glm::vec3 &Camera::getAcceleration() const
{ return m_acceleration; }

inline Camera::CameraBehavior Camera::getBehavior() const
{ return m_behavior; }

inline const glm::vec3 &Camera::getCurrentVelocity() const
{ return m_currentVelocity; }

inline const glm::vec3 &Camera::getPosition() const
{ return m_eye; }

//inline const glm::mat4x4 &Camera::getProjectionMatrix() const
//{ return m_projMatrix; }

inline const glm::vec3 &Camera::getVelocity() const
{ return m_velocity; }

inline const glm::vec3 &Camera::getViewDirection() const
{ return m_viewDir; }

//inline const glm::mat4x4 &Camera::getViewMatrix() const
//{ return m_viewMatrix; }

inline const glm::vec3 &Camera::getXAxis() const
{ return m_xAxis; }

inline const glm::vec3 &Camera::getYAxis() const
{ return m_yAxis; }

inline const glm::vec3 &Camera::getZAxis() const
{ return m_zAxis; }


#endif /* defined(__ADSPerFragment__Camera__) */
