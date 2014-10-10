#pragma once

#include "Models/SceneNode.h"

class Prism : public SceneNode
{
public:
    Prism();
    ~Prism();

    virtual void render();
};