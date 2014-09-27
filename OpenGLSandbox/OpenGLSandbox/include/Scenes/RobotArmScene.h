#pragma once

#include "Scene.h"

#include "Models/RobotArm.h"
#include "Renderer/Camera.h"
#include "Renderer/FreeCamera.h"

class RobotArmScene : public Scene
{
public:
    RobotArmScene();

    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

    virtual bool reshape(int width, int height);

private:
    float m_timePassed;

    Camera m_camera;
    FreeCamera m_freeCamera;
    RobotArm m_robotArm;
};