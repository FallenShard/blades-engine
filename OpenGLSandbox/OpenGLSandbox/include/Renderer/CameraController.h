#pragma once

#include "Renderer/FreeCamera.h"

namespace fsi
{

class AbstractCamera;
class ShaderProgram;
class Window;
struct Event;

class CameraController
{
public:
    CameraController(ShaderProgram* program, Window* window);
    ~CameraController();

    void setPosition(glm::vec3& position);
    void setSpeed(float translationSpeed, float lookSpeed);
    void setShaderProgram(ShaderProgram* program);

    bool handleEvents(const Event& event);
    void update(float timeDelta);
    void resize(int width, int height);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getCameraPosition() const;

private:
    void setFreeLook(bool freeLook);
    void adjustYawPitch(int mouseX, int mouseY);

    AbstractCamera* m_activeCamera;
    FreeCamera      m_freeCamera;

    float m_transSpeed;
    float m_lookSpeed;
    
    float m_yaw;
    float m_pitch;
    float m_roll;

    ShaderProgram*  m_program;

    bool    m_freeLook;
    Window* m_window;
};

}