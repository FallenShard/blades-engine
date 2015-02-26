#include "Models/TransformNode.h"

namespace fsi
{

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

}
