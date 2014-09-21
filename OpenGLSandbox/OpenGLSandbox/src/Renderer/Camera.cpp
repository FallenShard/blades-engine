#include "Renderer/Camera.h"

namespace
{
    const float degToRad = 3.141592f / 180.f;
}

Camera::Camera()
    : m_frustumScale(1.f)
    , m_zNear(1.f)
    , m_zFar(1000.f)
{
    setPerspectiveMatrix(m_zNear, m_zFar, m_frustumScale);
}

Camera::Camera(float fovDegrees)
    : m_zNear(1.f)
    , m_zFar(1000.f)
{
    float fovRad = fovDegrees * degToRad;
    m_frustumScale = 1.0f / tan(fovRad / 2.0f);

    setPerspectiveMatrix(m_zNear, m_zFar, m_frustumScale);
}

Camera::Camera(float zNear, float zFar, float frustumScale)
    : m_zNear(zNear)
    , m_zFar(zFar)
    , m_frustumScale(frustumScale)
{
    setPerspectiveMatrix(zNear, zFar, frustumScale);
}

void Camera::set(std::string name, ShaderProgram& program)
{
    program.getUniformAttribute(name);
    program.use();
    program.setUniformAttribute(name, 1, GL_FALSE, glm::value_ptr(m_perspectiveMatrix));
    ShaderProgram::release();
}

void Camera::setPerspectiveMatrix(float zNear, float zFar, float frustumScale)
{
    m_perspectiveMatrix[0].x = frustumScale;
    m_perspectiveMatrix[1].y = frustumScale;
    m_perspectiveMatrix[2].z = (zNear + zFar) / (zNear - zFar);
    m_perspectiveMatrix[2].w = -1.f;
    m_perspectiveMatrix[3].z = (2 * zNear * zFar) / (zNear - zFar);
}

void Camera::adjustViewport(int width, int height, ShaderProgram& program)
{
    m_perspectiveMatrix[0].x = m_frustumScale * (height / static_cast<float>(width));
    m_perspectiveMatrix[1].y = m_frustumScale;

    program.use();
    program.setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(m_perspectiveMatrix));
    ShaderProgram::release();

    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}