#include <algorithm>
#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Renderer/AbstractCamera.h"
#include "Window/Event.h"
#include "Window/Window.h"

#include "Renderer/CameraController.h"

CameraController::CameraController(ShaderProgram* program, Window* window)
    : m_activeCamera(&m_freeCamera)
    , m_program(program)
    , m_yaw(180.f)
    , m_pitch(0.f)
    , m_roll(0.f)
    , m_transSpeed(50.f)
    , m_lookSpeed(20.f)
    , m_freeLook(false)
    , m_window(window)
{
    m_freeCamera.setPosition(glm::vec3(0.f, 1.f, 1.f));
    m_freeCamera.rotate(m_yaw, m_pitch, 0.f);
    m_freeCamera.setSpeed(m_transSpeed);
}

CameraController::~CameraController()
{
}

void CameraController::setSpeed(float translationSpeed, float lookSpeed)
{
    m_transSpeed = translationSpeed;
    m_freeCamera.setSpeed(translationSpeed);

    m_lookSpeed = lookSpeed;
}

void CameraController::setShaderProgram(ShaderProgram* program)
{
    m_program = program;
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

        Vector2Di mousePos = Mouse::getPosition(*m_window);
        glm::ivec2 mPos(mousePos.getX(), mousePos.getY());

        if (!(windowCenter == mPos))
        {
            Mouse::setPosition(Vector2Di(windowCenter.x, windowCenter.y));
            m_activeCamera->rotate(m_yaw, m_pitch, 0.f);
        }
    }

    glm::vec3 prev = m_activeCamera->getPosition();

    if (Keyboard::isKeyPressed(Keyboard::Up))
        m_freeCamera.walk(timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::Down))
        m_freeCamera.walk(-timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::Right))
        m_freeCamera.strafe(timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        m_freeCamera.strafe(-timeDelta);

    if (prev != m_activeCamera->getPosition())
        std::cout << "CameraPos X:" << m_freeCamera.getPosition().x << " Y: " 
        << m_activeCamera->getPosition().y << " Z: " << m_activeCamera->getPosition().z << std::endl;

    m_program->use();
    m_program->setUniformAttribute("worldToCameraMatrix", 1, GL_FALSE, glm::value_ptr(m_activeCamera->getViewMatrix()));
    ShaderProgram::release();
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
    m_activeCamera->setupProjection(45, static_cast<GLfloat>(width) / height);

    m_program->use();
    m_program->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(m_activeCamera->getProjectionMatrix()));
    ShaderProgram::release();

    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}