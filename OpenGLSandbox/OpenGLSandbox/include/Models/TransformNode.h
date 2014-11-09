#pragma once

#include "Models/SceneNode.h"

class TransformNode : public SceneNode
{
public:
    TransformNode();
    ~TransformNode();

    virtual void render(const glm::mat4& projection, const glm::mat4& view);
};