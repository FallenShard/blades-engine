#pragma once

#include "Scene.h"

class PrismScene : public Scene
{
public:
    PrismScene();

    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

    virtual bool reshape(int width, int height);

private:
    float m_timePassed;
};