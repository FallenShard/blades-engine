#pragma once

#include <memory>
#include "Camera/FreeCamera.h"

namespace fsi
{

class AbstractCamera;
class ShaderProgram;
class Window;
struct Event;

class CameraController
{
public:
    CameraController(std::shared_ptr<Window>& window);
    ~CameraController();

    void setPosition(glm::vec3& position);
    void setSpeed(float translationSpeed, float lookSpeed);

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

    bool    m_freeLook;
    std::shared_ptr<Window> m_window;
};

}