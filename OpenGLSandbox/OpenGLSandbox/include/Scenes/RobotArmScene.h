#ifndef ROBOT_ARM_SCENE_H
#define ROBOT_ARM_SCENE_H

#include "Scene.h"

#include "Models/RobotArm.h"
#include "Renderer/Camera.h"

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
    RobotArm m_robotArm;
};

#endif // ROBOT_ARM_SCENE_H