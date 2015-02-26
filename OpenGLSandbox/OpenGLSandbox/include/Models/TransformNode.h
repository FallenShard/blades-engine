#pragma once

#include "Models/SceneNode.h"

namespace fsi
{

class TransformNode : public SceneNode
{
public:
    TransformNode();
    ~TransformNode();

    virtual void render(const glm::mat4& projection, const glm::mat4& view);
};

}