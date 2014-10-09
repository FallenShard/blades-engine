#pragma once

#include "Renderer/FreeCamera.h"

class AbstractCamera;
class ShaderProgram;
class Window;
struct Event;

class CameraController
{
public:
    CameraController(ShaderProgram* program, Window* window);
    ~CameraController();

    void setSpeed(float translationSpeed, float lookSpeed);
    void setShaderProgram(ShaderProgram* program);

    bool handleEvents(const Event& event);
    void update(float timeDelta);
    void resize(int width, int height);

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