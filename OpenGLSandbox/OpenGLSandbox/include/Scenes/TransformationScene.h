#ifndef TRANSFORMATION_SCENE_H
#define TRANSFORMATION_SCENE_H

#include "Scene.h"

class TransformationScene : public Scene
{
public:
    TransformationScene();

    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

private:
    float m_timePassed;
};

#endif // TRANSFORMATION_SCENE_H