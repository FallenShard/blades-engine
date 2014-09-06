#ifndef PRISM_SCENE_H
#define PRISM_SCENE_H

#include "Scene.h"

class PrismScene : public Scene
{
public:
    PrismScene();

    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

private:
    float m_timePassed;
};

#endif // PRISM_SCENE_H