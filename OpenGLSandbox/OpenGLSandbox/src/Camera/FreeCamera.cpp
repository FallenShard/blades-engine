#include "Camera/FreeCamera.h"

namespace fsi
{

FreeCamera::FreeCamera()
    : m_speed(0.5f)
    , m_translation(0.f)
{
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::update()
{
    glm::mat4 rotationMatrix = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
    m_position += m_translation;

    // set this when no movement decay is needed
    m_translation = glm::vec3(0);

    m_look  = glm::vec3(rotationMatrix * glm::vec4(0.f, 0.f, 1.f, 0.f));
    m_up    = glm::vec3(rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 0.f));
    m_right = glm::cross(m_look, m_up);

    glm::vec3 target = m_position + m_look;
    m_viewMatrix = glm::lookAt(m_position, target, m_up);
}

void FreeCamera::walk(const float timeDelta)
{
    m_translation += m_look * m_speed * timeDelta;
    update();
}

void FreeCamera::strafe(const float timeDelta)
{
    m_translation += m_right * m_speed * timeDelta;
    update();
}

void FreeCamera::lift(const float timeDelta)
{
    m_translation += m_up * m_speed * timeDelta;
    update();
}

void FreeCamera::setTranslation(const glm::vec3& transVec)
{
    m_translation = transVec;
    update();
}

glm::vec3 FreeCamera::getTranslation() const
{
    return m_translation;
}

void FreeCamera::setSpeed(const float speed)
{
    m_speed = speed;
}

const float FreeCamera::getSpeed() const
{
    return m_speed;
}

}