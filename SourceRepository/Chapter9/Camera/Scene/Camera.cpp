//
//  Camera.cpp
//  ADSPerFragment
//
//  Created by macbook on 2/2/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#include <cmath>
#include "Camera.h"
#include "Renderer.h"

const float Camera::DEFAULT_FOVX = 90.0f;
const float Camera::DEFAULT_ZFAR = 1000.0f;
const float Camera::DEFAULT_ZNEAR = 0.1f;
const glm::vec3 Camera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 Camera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);
const float EPSILON = 1e-6f;
const float PI = 3.1415926f;

static float degreesToRadians(float degrees)
{
    return (degrees * PI) / 180.0f;
}

static float radiansToDegrees(float radians)
{
    return (radians * 180.0f) / PI;
}

static bool closeEnough(float f1, float f2)
{
    // Determines whether the two floating-point values f1 and f2 are
    // close enough together that they can be considered equal.
    
    return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
}

Camera::Camera(Renderer* pObj)
{
    m_behavior = CAMERA_BEHAVIOR_FLIGHT;
    
    m_fovx = DEFAULT_FOVX;
    m_znear = DEFAULT_ZNEAR;
    m_zfar = DEFAULT_ZFAR;
    m_aspectRatio = 0.0f;
    
    m_accumPitchDegrees = 0.0f;
    
    m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
    m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    m_viewDir = glm::vec3(0.0f, 0.0f, -1.0f);
    
    m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    m_currentVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    parent = pObj;
    //m_viewMatrix.identity();
    //m_projMatrix.identity();
}

Camera::~Camera()
{
}

void Camera::lookAt(const glm::vec3 &target)
{
    lookAt(m_eye, target, m_yAxis);
}

void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
    m_eye = eye;
    
    m_zAxis = eye - target;
    m_zAxis = glm::normalize(m_zAxis);
    
    m_viewDir = -m_zAxis;
    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);
    
    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);
    m_xAxis = glm::normalize(m_xAxis);
    glm::mat4* m_viewMatrix = parent->RendererTransform()->TransformGetViewMatrix();

    (*m_viewMatrix)[0][0] = m_xAxis.x;
    (*m_viewMatrix)[1][0] = m_xAxis.y;
    (*m_viewMatrix)[2][0] = m_xAxis.z;
    (*m_viewMatrix)[3][0] = -glm::dot(m_xAxis, eye);
    
    (*m_viewMatrix)[0][1] = m_yAxis.x;
    (*m_viewMatrix)[1][1] = m_yAxis.y;
    (*m_viewMatrix)[2][1] = m_yAxis.z;
    (*m_viewMatrix)[3][1] = -glm::dot(m_yAxis, eye);
    
    (*m_viewMatrix)[0][2] = m_zAxis.x;
    (*m_viewMatrix)[1][2] = m_zAxis.y;
    (*m_viewMatrix)[2][2] = m_zAxis.z;
    (*m_viewMatrix)[3][2] = -glm::dot(m_zAxis, eye);
    
    // Extract the pitch angle from the view matrix.
    m_accumPitchDegrees = radiansToDegrees(-asinf((*m_viewMatrix)[1][2]));
}

void Camera::move(float dx, float dy, float dz)
{
    // Moves the camera by dx world units to the left or right; dy
    // world units upwards or downwards; and dz world units forwards
    // or backwards.
    
    glm::vec3 eye = m_eye;
    glm::vec3 forwards;
    
    if (m_behavior == CAMERA_BEHAVIOR_FIRST_PERSON)
    {
        // Calculate the forwards direction. Can't just use the camera's local
        // z axis as doing so will cause the camera to move more slowly as the
        // camera's view approaches 90 degrees straight up and down.
        
        forwards = glm::cross(WORLD_YAXIS, m_xAxis);
        forwards = glm::normalize(forwards);
    }
    else
    {
        forwards = m_viewDir;
    }
    
    eye += m_xAxis * dx;
    eye += WORLD_YAXIS * dy;
    eye += forwards * dz;
    
    setPosition(eye);
}

void Camera::move(const glm::vec3 &direction, const glm::vec3 &amount)
{
    // Moves the camera by the specified amount of world units in the specified
    // direction in world space.
    
    m_eye.x += direction.x * amount.x;
    m_eye.y += direction.y * amount.y;
    m_eye.z += direction.z * amount.z;
    
    updateViewMatrix(false);
}

/*void Camera::perspective(float fovx, float aspect, float znear, float zfar)
{
    // Construct a projection matrix based on the horizontal field of view
    // 'fovx' rather than the more traditional vertical field of view 'fovy'.
    
    float e = 1.0f / tanf(degreesToRadians(fovx) / 2.0f);
    float aspectInv = 1.0f / aspect;
    float fovy = 2.0f * atanf(aspectInv / e);
    float xScale = 1.0f / tanf(0.5f * fovy);
    float yScale = xScale / aspectInv;
    
    m_projMatrix[0][0] = xScale;
    m_projMatrix[0][1] = 0.0f;
    m_projMatrix[0][2] = 0.0f;
    m_projMatrix[0][3] = 0.0f;
    
    m_projMatrix[1][0] = 0.0f;
    m_projMatrix[1][1] = yScale;
    m_projMatrix[1][2] = 0.0f;
    m_projMatrix[1][3] = 0.0f;
    
    m_projMatrix[2][0] = 0.0f;
    m_projMatrix[2][1] = 0.0f;
    m_projMatrix[2][2] = (zfar + znear) / (znear - zfar);
    m_projMatrix[2][3] = -1.0f;
    
    m_projMatrix[3][0] = 0.0f;
    m_projMatrix[3][1] = 0.0f;
    m_projMatrix[3][2] = (2.0f * zfar * znear) / (znear - zfar);
    m_projMatrix[3][3] = 0.0f;
    
    m_fovx = fovx;
    m_aspectRatio = aspect;
    m_znear = znear;
    m_zfar = zfar;
}*/

void Camera::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // Rotates the camera based on its current behavior.
    // Note that not all behaviors support rolling.
    
    switch (m_behavior)
    {
        case CAMERA_BEHAVIOR_FIRST_PERSON:
            rotateFirstPerson(headingDegrees, pitchDegrees);
            break;
            
        case CAMERA_BEHAVIOR_FLIGHT:
            rotateFlight(headingDegrees, pitchDegrees, rollDegrees);
            break;
    }
    
    updateViewMatrix(true);
}

void Camera::rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    glm::mat4x4 rotMtx;
    
    // Rotate camera's existing x and z axes about its existing y axis.
    if (headingDegrees != 0.0f)
    {
        //rotMtx.rotate(m_yAxis, headingDegrees);
        rotMtx = glm::rotate(rotMtx, headingDegrees, m_yAxis);
        m_xAxis = m_xAxis * glm::mat3(rotMtx);
        m_zAxis = m_zAxis * glm::mat3(rotMtx);
    }
    
    // Rotate camera's existing y and z axes about its existing x axis.
    if (pitchDegrees != 0.0f)
    {
        rotMtx = glm::rotate(rotMtx, pitchDegrees, m_xAxis);
        m_yAxis = m_yAxis * glm::mat3(rotMtx);
        m_zAxis = m_zAxis * glm::mat3(rotMtx);
        
        //rotMtx.rotate(m_xAxis, pitchDegrees);
        //m_yAxis = m_yAxis * rotMtx;
        //m_zAxis = m_zAxis * rotMtx;
    }
    
    // Rotate camera's existing x and y axes about its existing z axis.
    if (rollDegrees != 0.0f)
    {
        rotMtx = glm::rotate(rotMtx, rollDegrees, m_zAxis);
        m_xAxis = m_xAxis * glm::mat3(rotMtx);
        m_yAxis = m_yAxis * glm::mat3(rotMtx);

        //rotMtx.rotate(m_zAxis, rollDegrees);
        //m_xAxis = m_xAxis * rotMtx;
        //m_yAxis = m_yAxis * rotMtx;
    }
}

void Camera::rotateFirstPerson(float headingDegrees, float pitchDegrees)
{
    m_accumPitchDegrees += pitchDegrees;
    
    if (m_accumPitchDegrees > 90.0f)
    {
        pitchDegrees = 90.0f - (m_accumPitchDegrees - pitchDegrees);
        m_accumPitchDegrees = 90.0f;
    }
    
    if (m_accumPitchDegrees < -90.0f)
    {
        pitchDegrees = -90.0f - (m_accumPitchDegrees - pitchDegrees);
        m_accumPitchDegrees = -90.0f;
    }
    
    glm::mat4x4 rotMtx;
    
    // Rotate camera's existing x and z axes about the world y axis.
    if (headingDegrees != 0.0f)
    {
        rotMtx = glm::rotate(rotMtx, headingDegrees, WORLD_YAXIS);
        m_xAxis = m_xAxis * glm::mat3(rotMtx);
        m_zAxis = m_zAxis * glm::mat3(rotMtx);
        
        //rotMtx.rotate(WORLD_YAXIS, headingDegrees);
        //m_xAxis = m_xAxis * rotMtx;
        //m_zAxis = m_zAxis * rotMtx;
    }
    
    // Rotate camera's existing y and z axes about its existing x axis.
    if (pitchDegrees != 0.0f)
    {
        rotMtx = glm::rotate(rotMtx, pitchDegrees, m_xAxis);
        m_yAxis = m_yAxis * glm::mat3(rotMtx);
        m_zAxis = m_zAxis * glm::mat3(rotMtx);

        //rotMtx.rotate(m_xAxis, pitchDegrees);
        //m_yAxis = m_yAxis * rotMtx;
        //m_zAxis = m_zAxis * rotMtx;
    }
}

void Camera::setAcceleration(float x, float y, float z)
{
    m_acceleration = glm::vec3(x, y, z);
}

void Camera::setAcceleration(const glm::vec3 &acceleration)
{
    m_acceleration = acceleration;
}

void Camera::setBehavior(CameraBehavior newBehavior)
{
    if (m_behavior == CAMERA_BEHAVIOR_FLIGHT && newBehavior == CAMERA_BEHAVIOR_FIRST_PERSON)
    {
        // Moving from flight-simulator mode to first-person.
        // Need to ignore camera roll, but retain existing pitch and heading.
        lookAt(m_eye, m_eye + (-m_zAxis), WORLD_YAXIS);//Parminder here inverse function is removed. need to check if '-' gives the expected behaviour of inverse()
    }
    
    m_behavior = newBehavior;
}

void Camera::setCurrentVelocity(float x, float y, float z)
{
    m_currentVelocity = glm::vec3(x, y, z);
}

void Camera::setCurrentVelocity(const glm::vec3 &currentVelocity)
{
    m_currentVelocity = currentVelocity;
}

void Camera::setPosition(float x, float y, float z)
{
    m_eye = glm::vec3(x, y, z);
    updateViewMatrix(false);
}

void Camera::setPosition(const glm::vec3 &position)
{
    m_eye = position;
    updateViewMatrix(false);
}

void Camera::setVelocity(float x, float y, float z)
{
    m_velocity = glm::vec3(x, y, z);
}

void Camera::setVelocity(const glm::vec3 &velocity)
{
    m_velocity = velocity;
}

void Camera::updatePosition(const glm::vec3 &direction, float elapsedTimeSec)
{
    // Moves the camera using Newton's second law of motion. Unit mass is
    // assumed here to somewhat simplify the calculations. The direction vector
    // is in the range [-1,1].
    float magSquare = (m_currentVelocity.x * m_currentVelocity.x) + (m_currentVelocity.y * m_currentVelocity.y) + (m_currentVelocity.z * m_currentVelocity.z);
    
    if (magSquare != 0.0f)
    {
        // Only move the camera if the velocity vector is not of zero length.
        // Doing this guards against the camera slowly creeping around due to
        // floating point rounding errors.
        
        glm::vec3 displacement = (m_currentVelocity * elapsedTimeSec) +
        (0.5f * m_acceleration * elapsedTimeSec * elapsedTimeSec);
        
        // Floating point rounding errors will slowly accumulate and cause the
        // camera to move along each axis. To prevent any unintended movement
        // the displacement vector is clamped to zero for each direction that
        // the camera isn't moving in. Note that the updateVelocity() method
        // will slowly decelerate the camera's velocity back to a stationary
        // state when the camera is no longer moving along that direction. To
        // account for this the camera's current velocity is also checked.
        
        if (direction.x == 0.0f && closeEnough(m_currentVelocity.x, 0.0f))
            displacement.x = 0.0f;
        
        if (direction.y == 0.0f && closeEnough(m_currentVelocity.y, 0.0f))
            displacement.y = 0.0f;
        
        if (direction.z == 0.0f && closeEnough(m_currentVelocity.z, 0.0f))
            displacement.z = 0.0f;
        
        move(displacement.x, displacement.y, displacement.z);
    }
    
    // Continuously update the camera's velocity vector even if the camera
    // hasn't moved during this call. When the camera is no longer being moved
    // the camera is decelerating back to its stationary state.
    
    updateVelocity(direction, elapsedTimeSec);
}

void Camera::updateVelocity(const glm::vec3 &direction, float elapsedTimeSec)
{
    // Updates the camera's velocity based on the supplied movement direction
    // and the elapsed time (since this method was last called). The movement
    // direction is in the range [-1,1].
    
    if (direction.x != 0.0f)
    {
        // Camera is moving along the x axis.
        // Linearly accelerate up to the camera's max speed.
        
        m_currentVelocity.x += direction.x * m_acceleration.x * elapsedTimeSec;
        
        if (m_currentVelocity.x > m_velocity.x)
            m_currentVelocity.x = m_velocity.x;
        else if (m_currentVelocity.x < -m_velocity.x)
            m_currentVelocity.x = -m_velocity.x;
    }
    else
    {
        // Camera is no longer moving along the x axis.
        // Linearly decelerate back to stationary state.
        
        if (m_currentVelocity.x > 0.0f)
        {
            if ((m_currentVelocity.x -= m_acceleration.x * elapsedTimeSec) < 0.0f)
                m_currentVelocity.x = 0.0f;
        }
        else
        {
            if ((m_currentVelocity.x += m_acceleration.x * elapsedTimeSec) > 0.0f)
                m_currentVelocity.x = 0.0f;
        }
    }
    
    if (direction.y != 0.0f)
    {
        // Camera is moving along the y axis.
        // Linearly accelerate up to the camera's max speed.
        
        m_currentVelocity.y += direction.y * m_acceleration.y * elapsedTimeSec;
        
        if (m_currentVelocity.y > m_velocity.y)
            m_currentVelocity.y = m_velocity.y;
        else if (m_currentVelocity.y < -m_velocity.y)
            m_currentVelocity.y = -m_velocity.y;
    }
    else
    {
        // Camera is no longer moving along the y axis.
        // Linearly decelerate back to stationary state.
        
        if (m_currentVelocity.y > 0.0f)
        {
            if ((m_currentVelocity.y -= m_acceleration.y * elapsedTimeSec) < 0.0f)
                m_currentVelocity.y = 0.0f;
        }
        else
        {
            if ((m_currentVelocity.y += m_acceleration.y * elapsedTimeSec) > 0.0f)
                m_currentVelocity.y = 0.0f;
        }
    }
    
    if (direction.z != 0.0f)
    {
        // Camera is moving along the z axis.
        // Linearly accelerate up to the camera's max speed.
        
        m_currentVelocity.z += direction.z * m_acceleration.z * elapsedTimeSec;
        
        if (m_currentVelocity.z > m_velocity.z)
            m_currentVelocity.z = m_velocity.z;
        else if (m_currentVelocity.z < -m_velocity.z)
            m_currentVelocity.z = -m_velocity.z;
    }
    else
    {
        // Camera is no longer moving along the z axis.
        // Linearly decelerate back to stationary state.
        
        if (m_currentVelocity.z > 0.0f)
        {
            if ((m_currentVelocity.z -= m_acceleration.z * elapsedTimeSec) < 0.0f)
                m_currentVelocity.z = 0.0f;
        }
        else
        {
            if ((m_currentVelocity.z += m_acceleration.z * elapsedTimeSec) > 0.0f)
                m_currentVelocity.z = 0.0f;
        }
    }
}

void Camera::updateViewMatrix(bool orthogonalizeAxes)
{
    if (orthogonalizeAxes)
    {
        // Regenerate the camera's local axes to orthogonalize them.
        m_zAxis = glm::normalize(m_zAxis);
        //m_zAxis.normalize();
 
        m_yAxis = glm::cross(m_zAxis, m_xAxis);
        m_yAxis = glm::normalize(m_yAxis);
        //m_yAxis = glm::vec3::cross(m_zAxis, m_xAxis);
        //m_yAxis.normalize();
        
        m_xAxis = glm::cross(m_yAxis, m_zAxis);
        m_xAxis = glm::normalize(m_xAxis);
        //m_xAxis = glm::vec3::cross(m_yAxis, m_zAxis);
        //m_xAxis.normalize();
        
        m_viewDir = -m_zAxis;
    }
    
    // Reconstruct the view matrix.
    glm::mat4* m_viewMatrix = parent->RendererTransform()->TransformGetViewMatrix();
    
    (*m_viewMatrix)[0][0] = m_xAxis.x;
    (*m_viewMatrix)[1][0] = m_xAxis.y;
    (*m_viewMatrix)[2][0] = m_xAxis.z;
    (*m_viewMatrix)[3][0] = -glm::dot(m_xAxis, m_eye);
    
    (*m_viewMatrix)[0][1] = m_yAxis.x;
    (*m_viewMatrix)[1][1] = m_yAxis.y;
    (*m_viewMatrix)[2][1] = m_yAxis.z;
    (*m_viewMatrix)[3][1] = -glm::dot(m_yAxis, m_eye);
    
    (*m_viewMatrix)[0][2] = m_zAxis.x;    
    (*m_viewMatrix)[1][2] = m_zAxis.y;
    (*m_viewMatrix)[2][2] = m_zAxis.z;   
    (*m_viewMatrix)[3][2] = -glm::dot(m_zAxis, m_eye);
    
    (*m_viewMatrix)[0][3] = 0.0f;
    (*m_viewMatrix)[1][3] = 0.0f;
    (*m_viewMatrix)[2][3] = 0.0f;
    (*m_viewMatrix)[3][3] = 1.0f;
    /*LOGI("00: %f",(*m_viewMatrix)[0][0]);
    LOGI("10: %f",(*m_viewMatrix)[1][0]);
    LOGI("20: %f",(*m_viewMatrix)[2][0]);
    LOGI("30: %f",(*m_viewMatrix)[3][0]);
    
    LOGI("01: %f",(*m_viewMatrix)[0][1]);
    LOGI("11: %f",(*m_viewMatrix)[1][1]);
    LOGI("21: %f",(*m_viewMatrix)[2][1]);
    LOGI("31: %f",(*m_viewMatrix)[3][1]);
    
    LOGI("02: %f",(*m_viewMatrix)[0][2]);
    LOGI("12: %f",(*m_viewMatrix)[1][2]);
    LOGI("22: %f",(*m_viewMatrix)[2][2]);
    LOGI("32: %f",(*m_viewMatrix)[3][2]);
    
    LOGI("03: %f",(*m_viewMatrix)[0][3]);
    LOGI("13: %f",(*m_viewMatrix)[1][3]);
    LOGI("23: %f",(*m_viewMatrix)[2][3]);
    LOGI("33: %f",(*m_viewMatrix)[3][3]);*/
    
}