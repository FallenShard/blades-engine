#pragma once

#include <random>
#include <functional>
#include <chrono>
#include "Scene.h"

class GraphScene : public Scene
{
public:
    GraphScene();

    virtual void prepare();
    virtual void handleEvents(const Event& event);
    virtual void update(float timeDelta);
    virtual void render();

    virtual bool reshape(int width, int height);

private:
    void randomize(unsigned int seed);
    void TSPIteration();

    float m_timePassed;
    bool m_depthClamp;

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;
};