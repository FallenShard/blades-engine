#ifndef TRIANGLE_SCENE_H
#define TRIANGLE_SCENE_H

#include "Scene.h"

class TriangleScene : public Scene
{
public:
    TriangleScene();
    
    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

private:
    float m_timePassed;
};

#endif