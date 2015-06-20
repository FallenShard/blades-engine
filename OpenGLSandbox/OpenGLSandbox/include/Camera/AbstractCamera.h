#pragma once

#include "OpenGL.h"

namespace fsi
{

class AbstractCamera
{
public:
    // Default constructor, initializes to default values
    AbstractCamera();
    ~AbstractCamera();

    void setupProjection(const float fovY, const float aspectRatio, const float zNear = 0.1f, const float zFar = 1000.f);
    
    virtual void update() = 0;
    virtual void rotate(const float yaw, const float pitch, const float roll);

    const glm::mat4 getViewMatrix() const;
    const glm::mat4 getProjectionMatrix() const;

    void setPosition(const glm::vec3& position);
    const glm::vec3 getPosition() const;
    const glm::vec3 getViewDirection() const;

    void setFov(const float yDegrees);
    const float getFov() const;

    const float getAspectRatio() const;
    
    void calcFrustumPlanes();

    bool isPointInFrustum(const glm::vec3& point);
    bool isSphereInFrustum(const glm::vec3& center, const float radius);
    bool isBoxInFrustum(const glm::vec3& min, const glm::vec3& max);

    void getFrustumPlanes(glm::vec4 planes[6]);

    glm::vec3 farPoints[4];
    glm::vec4 nearPoints[4];

protected:
    float m_yaw;
    float m_pitch;
    float m_roll;

    float m_fov;
    float m_aspectRatio;
    float m_zNear;
    float m_zFar;

    glm::vec3 m_look;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_position;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};

}