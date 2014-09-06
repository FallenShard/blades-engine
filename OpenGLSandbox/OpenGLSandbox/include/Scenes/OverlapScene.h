#ifndef OVERLAP_SCENE_H
#define OVERLAP_SCENE_H

#include "Scene.h"

class OverlapScene : public Scene
{
public:
    OverlapScene();

    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

private:
    float m_timePassed;
    bool m_depthClamp;
};

#endif // OVERLAP_SCENE_H