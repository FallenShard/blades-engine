#include "Models/SceneNode.h"
#include <iostream>

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
    for (auto& child : m_children)
        delete child;
    m_children.clear();
}

//void SceneNode::attachChild(SceneNode::NodePtr nodePointer)
//{
//    nodePointer->attachParent(this);
//    m_children.push_back(nodePointer);
//}

void SceneNode::attachChild(SceneNode* child)
{
    child->attachParent(this);
    child->setVertexArray(m_vertexArray);
    child->setShaderProgram(m_shaderProgram);
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

void SceneNode::translate(const glm::vec3& position)
{
    m_position += position;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
}

void SceneNode::rotateX(const float angle)
{
    m_rotation.x += angle;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(1.f, 0.f, 0.f));
    std::cout << "rotX: " << m_rotation.x << " rotY: " << m_rotation.y << " rotZ: " << m_rotation.z << std::endl;
}

void SceneNode::rotateY(const float angle)
{
    m_rotation.y += angle;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
    std::cout << "rotX: " << m_rotation.x << " rotY: " << m_rotation.y << " rotZ: " << m_rotation.z << std::endl;
}

void SceneNode::rotateZ(const float angle)
{
    m_rotation.z += angle;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
    std::cout << "rotX: " << m_rotation.x << " rotY: " << m_rotation.y << " rotZ: " << m_rotation.z << std::endl;
}

void SceneNode::scale(const glm::vec3& scale)
{
    m_scale += scale;
    m_modelMatrix = glm::scale(m_modelMatrix, scale);
}

void SceneNode::applyTransformation()
{
    applyTransformation(glm::mat4(1.f));
}

void SceneNode::applyTransformation(const glm::mat4& transformation)
{
    m_modelMatrix = transformation * m_modelMatrix;

    for (auto& child : m_children)
        child->applyTransformation(m_modelMatrix);
}

void SceneNode::renderSelf()
{
    m_vertexArray->bind();
    m_vertexArray->renderIndexed();
}

void SceneNode::renderChildren()
{
    for (auto& child : m_children)
        child->render();
}