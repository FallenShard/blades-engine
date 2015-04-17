#include <algorithm>
#include <iostream>

#include "OglWrapper/ShaderProgram.h"

#include "Window/Window.h"
#include "Input/Event.h"

#include "Camera/AbstractCamera.h"

#include "Camera/CameraController.h"

namespace fsi
{

CameraController::CameraController(Window* window)
    : m_activeCamera(&m_freeCamera)
    , m_yaw(180.f)
    , m_pitch(0.f)
    , m_roll(0.f)
    , m_transSpeed(10.f)
    , m_lookSpeed(10.f)
    , m_freeLook(false)
    , m_window(window)
{
    m_freeCamera.setPosition(glm::vec3(0.f, -40.f, 60.f));
    m_freeCamera.rotate(m_yaw, m_pitch, 0.f);
    m_freeCamera.setSpeed(m_transSpeed);
}

CameraController::~CameraController()
{
}

void CameraController::setPosition(glm::vec3& position)
{
    m_activeCamera->setPosition(position);
}

void CameraController::setSpeed(float translationSpeed, float lookSpeed)
{
    m_transSpeed = translationSpeed;
    m_freeCamera.setSpeed(translationSpeed);

    m_lookSpeed = lookSpeed;
}

bool CameraController::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::MouseButtonPressed:
    {
        if (event.mouseButton.button == Mouse::Right)
            setFreeLook(true);

        break;
    }

    case Event::MouseMoved:
    {
        if (m_freeLook == true)
            adjustYawPitch(event.mouseMove.x, event.mouseMove.y);

        break;
    }

    case Event::MouseButtonReleased:
    {
        if (event.mouseButton.button == Mouse::Right)
            setFreeLook(false);

        break;
    }

    case Event::Resized:
    {
        resize(event.size.width, event.size.height);

        break;
    }
    };

    return true;
}

void CameraController::update(float timeDelta)
{
    if (m_freeLook)
    {
        glm::ivec2 windowPos = m_window->getPosition();
        glm::ivec2 windowSize = m_window->getSize();
        glm::ivec2 windowCenter = windowPos + windowSize / 2;

        glm::ivec2 mousePos = Mouse::getPosition(*m_window);
        glm::ivec2 mPos(mousePos.x, mousePos.y);

        if (!(windowCenter == mPos))
        {
            Mouse::setPosition(glm::ivec2(windowCenter.x, windowCenter.y));
            m_activeCamera->rotate(m_yaw, m_pitch, 0.f);
        }
    }

    glm::vec3 prev = m_activeCamera->getPosition();

    if (Keyboard::isKeyPressed(Keyboard::W))
        m_freeCamera.walk(timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::S))
        m_freeCamera.walk(-timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::D))
        m_freeCamera.strafe(timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::A))
        m_freeCamera.strafe(-timeDelta);

#ifdef _DEBUG
    if (prev != m_activeCamera->getPosition())
    {
        std::cout << "CameraPos X:" << m_freeCamera.getPosition().x << " Y: "
            << m_activeCamera->getPosition().y << " Z: " << m_activeCamera->getPosition().z << std::endl;
    }
#endif
}

void CameraController::setFreeLook(bool freeLook)
{
    m_freeLook = freeLook;
    m_window->showMouseCursor(!freeLook);
}

void CameraController::adjustYawPitch(int mouseX, int mouseY)
{
    glm::ivec2 windowCenter = m_window->getSize() / 2;
    glm::ivec2 mousePos(mouseX, mouseY);

    m_pitch += (mousePos.y - windowCenter.y) / m_lookSpeed;
    m_pitch = std::max(-90.f, std::min(m_pitch, 90.f));

    m_yaw += (windowCenter.x - mousePos.x) / m_lookSpeed;
}

void CameraController::resize(int width, int height)
{
    m_activeCamera->setupProjection(45, static_cast<GLfloat>(width) / height, 1.f, 8192.f);

    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

glm::mat4 CameraController::getViewMatrix() const
{
    return m_activeCamera->getViewMatrix();
}

glm::mat4 CameraController::getProjectionMatrix() const
{
    return m_activeCamera->getProjectionMatrix();
}

glm::vec3 CameraController::getCameraPosition() const
{
    return m_activeCamera->getPosition();
}

}