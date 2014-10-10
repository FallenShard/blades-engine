#include "Models/SceneNode.h"

SceneNode::SceneNode()
    : m_modelMatrix(1.f)
    , m_position(0.f)
    , m_scale(1.f)
    , m_rotation(0.f)
{
}

SceneNode::SceneNode(VertexArray* vArray, ShaderProgram* program)
    : m_modelMatrix(1.f)
    , m_position(0.f)
    , m_scale(1.f)
    , m_rotation(0.f)
    , m_vertexArray(vArray)
    , m_shaderProgram(program)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::attachChild(SceneNode::NodePtr nodePointer)
{
    nodePointer->attachParent(this);
    m_children.push_back(nodePointer);
}

void SceneNode::attachParent(SceneNode* parent)
{
    m_parent = parent;
}

void SceneNode::translate(const glm::vec3& position)
{
    m_position += position;
    m_modelMatrix = glm::translate(m_modelMatrix, position) * m_modelMatrix;
}

void SceneNode::rotateX(const float angle)
{
    m_rotation.x += angle;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(1.f, 0.f, 0.f)) * m_modelMatrix;
}

void SceneNode::rotateY(const float angle)
{
    m_rotation.y += angle;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f)) * m_modelMatrix;
}

void SceneNode::rotateZ(const float angle)
{
    m_rotation.z += angle;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(1.f, 0.f, 1.f)) * m_modelMatrix;
}

void SceneNode::scale(const glm::vec3& scale)
{
    m_scale += scale;
    m_modelMatrix = glm::scale(m_modelMatrix, scale) * m_modelMatrix;
}

void SceneNode::applyTransformation(const glm::mat4& transformation)
{
    m_modelMatrix = transformation * m_modelMatrix;

    for (auto& child : m_children)
        child->applyTransformation(m_modelMatrix);
}

void SceneNode::render()
{
    renderSelf();
    renderChildren();
}

void SceneNode::renderSelf()
{

}

void SceneNode::renderChildren()
{

}