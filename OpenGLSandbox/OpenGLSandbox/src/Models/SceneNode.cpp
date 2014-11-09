#include "Models/SceneNode.h"
#include <iostream>

SceneNode::SceneNode()
    : m_absoluteTrans(1.f)
    , m_relativeTrans(1.f)
    , m_position(0.f)
    , m_scale(1.f)
    , m_rotation()
    , m_needUpdate(true)
{
}

SceneNode::SceneNode(VertexArray* vArray, ShaderProgram* program)
    : m_absoluteTrans(1.f)
    , m_relativeTrans(1.f)
    , m_position(0.f)
    , m_scale(1.f)
    , m_rotation()
    , m_vertexArray(vArray)
    , m_shaderProgram(program)
    , m_needUpdate(true)
{
}

SceneNode::~SceneNode()
{
    for (auto& child : m_children)
        delete child;
    m_children.clear();
}

void SceneNode::attachChild(SceneNode* child)
{
    child->attachParent(this);
    m_children.push_back(child);
}

void SceneNode::attachParent(SceneNode* parent)
{
    m_parent = parent;
}

void SceneNode::setVertexArray(VertexArray* vArray)
{
    m_vertexArray = vArray;
}

void SceneNode::setShaderProgram(ShaderProgram* program)
{
    m_shaderProgram = program;
}

void SceneNode::setUpdateStatus(bool status)
{
    m_needUpdate = status;
}

void SceneNode::setPosition(const glm::vec3& position)
{
    m_position = position;
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::translate(const glm::vec3& translation)
{
    m_position += translation;
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::rotate(const float angle, const glm::vec3& axis)
{
    m_rotation = glm::rotate(m_rotation, glm::radians(angle), axis);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::setRotationX(const float angle)
{
    m_rotation.x = glm::radians(angle);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::rotateX(const float angle)
{
    m_rotation.x += glm::radians(angle);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::setRotationY(const float angle)
{
    m_rotation.y = glm::radians(angle);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::rotateY(const float angle)
{
    m_rotation.y += glm::radians(angle);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::setRotationZ(const float angle)
{
    m_rotation.z = glm::radians(angle);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::rotateZ(const float angle)
{
    m_rotation.z += glm::radians(angle);
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::setTransformationMatrix(glm::mat4& matrix)
{
    m_relativeTrans = matrix;
    updateAbsoluteTrans();
}

void SceneNode::scale(const glm::vec3& scale)
{
    m_scale += scale;
    updateRelativeTrans();
    updateAbsoluteTrans();
}

void SceneNode::applyTransformation(const glm::mat4& transformation)
{         
    m_absoluteTrans = transformation * m_relativeTrans;

    for (auto& child : m_children)
        child->applyTransformation(m_absoluteTrans);
}

const glm::mat4& SceneNode::getModelMatrix() const
{
    return m_absoluteTrans;
}

void SceneNode::updateRelativeTrans()
{
    m_relativeTrans = glm::scale(m_scale);
    m_relativeTrans = glm::rotate(m_rotation.x, glm::vec3(1.f, 0.f, 0.f)) * m_relativeTrans;
    m_relativeTrans = glm::rotate(m_rotation.y, glm::vec3(0.f, 1.f, 0.f)) * m_relativeTrans;
    m_relativeTrans = glm::rotate(m_rotation.z, glm::vec3(0.f, 0.f, 1.f)) * m_relativeTrans;
    m_relativeTrans = glm::translate(m_position) * m_relativeTrans;
}

void SceneNode::updateAbsoluteTrans()
{
    if (m_parent)
        applyTransformation(m_parent->m_absoluteTrans);
    else
        applyTransformation();
}

void SceneNode::renderChildren(const glm::mat4& projection, const glm::mat4& view)
{
    for (auto& child : m_children)
        child->render(projection, view);
}