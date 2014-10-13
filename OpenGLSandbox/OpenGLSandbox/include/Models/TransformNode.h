#pragma once

#include "Models/SceneNode.h"

class TransformNode : public SceneNode
{
public:
    TransformNode();
    ~TransformNode();

    virtual void render();
};