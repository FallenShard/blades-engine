#include "Models/TransformNode.h"

TransformNode::TransformNode()
{
}

TransformNode::~TransformNode()
{
}

void TransformNode::render(const glm::mat4& projection, const glm::mat4& view)
{
    renderChildren(projection, view);
}
