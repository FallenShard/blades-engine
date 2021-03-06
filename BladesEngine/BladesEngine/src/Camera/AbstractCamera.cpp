#include "Camera/AbstractCamera.h"

namespace fsi
{
    AbstractCamera::AbstractCamera()
        : m_zNear(1.f)
        , m_zFar(8192.f)
        , m_fov(glm::radians(45.f))
        , m_aspectRatio(1.f)
        , m_up(0.f, 1.f, 0.f)
    {
    }

    AbstractCamera::~AbstractCamera()
    {
    }

    void AbstractCamera::setupProjection(const float fovY, const float aspectRatio, const float zNear, const float zFar)
    {
        m_fov = glm::radians(fovY);
        m_aspectRatio = aspectRatio;
        m_zNear = zNear;
        m_zFar = zFar;
        m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_zNear, m_zFar);
    }

    void AbstractCamera::rotate(const float yaw, const float pitch, const float roll)
    {
        m_yaw = glm::radians(yaw);
        m_pitch = glm::radians(pitch);
        m_roll = glm::radians(roll);
    
        update();
    }

    const glm::mat4 AbstractCamera::getViewMatrix() const
    {
        return m_viewMatrix;
    }

    const glm::mat4 AbstractCamera::getProjectionMatrix() const
    {
        return m_projectionMatrix;
    }

    void AbstractCamera::setPosition(const glm::vec3& position)
    {
        m_position = position;

        update();
    }

    const glm::vec3 AbstractCamera::getPosition() const
    {
        return m_position;
    }

    const glm::vec3 AbstractCamera::getViewDirection() const
    {
        return m_look;
    }

    const float AbstractCamera::getFov() const
    {
        return m_fov;
    }

    void AbstractCamera::setFov(const float yDegrees)
    {
        m_fov = glm::radians(yDegrees);
        m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_zNear, m_zFar);
    }

    void AbstractCamera::setAspectRatio(const float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_zNear, m_zFar);
    }

    const float AbstractCamera::getAspectRatio() const
    {
        return m_aspectRatio;
    }

    void AbstractCamera::calcFrustumPlanes()
    {
        // TO DO
    }

    bool AbstractCamera::isPointInFrustum(const glm::vec3& point)
    {
        return false;
    }

    bool AbstractCamera::isSphereInFrustum(const glm::vec3& center, const float radius)
    {
        return false;
    }

    bool AbstractCamera::isBoxInFrustum(const glm::vec3& min, const glm::vec3& max)
    {
        return false;
    }

    void AbstractCamera::getFrustumPlanes(glm::vec4 planes[6])
    {
    }
}