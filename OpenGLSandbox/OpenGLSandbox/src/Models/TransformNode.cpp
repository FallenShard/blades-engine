#include "Models/TransformNode.h"

TransformNode::TransformNode()
{
}

TransformNode::~TransformNode()
{
}

void TransformNode::render()
{
    renderChildren();
}
