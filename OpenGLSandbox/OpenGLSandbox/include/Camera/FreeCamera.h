#pragma once

#include "Camera/AbstractCamera.h"

namespace fsi
{

class FreeCamera : public AbstractCamera
{
public:
    FreeCamera();
    ~FreeCamera();

    virtual void update();

    void walk(const float timeDelta);
    void strafe(const float timeDelta);
    void lift(const float timeDelta);

    void setTranslation(const glm::vec3& transVec);
    glm::vec3 getTranslation() const;

    void setSpeed(const float speed);
    const float getSpeed() const;

private:
    float m_speed;
    glm::vec3 m_translation;

};

}