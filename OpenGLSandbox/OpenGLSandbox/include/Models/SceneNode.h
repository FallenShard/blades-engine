#pragma once

#include "glm.hpp"
#include <list>
#include <memory>

#include "Renderer/VertexArray.h"
#include "Renderer/ShaderProgram.h"

class SceneNode
{
public:
    //typedef std::shared_ptr<SceneNode> NodePtr;
    //typedef std::list<NodePtr>          NodeList;

    SceneNode();
    SceneNode(VertexArray* vArray, ShaderProgram* program);
    ~SceneNode();

    //void attachChild(NodePtr nodePointer);
    void attachChild(SceneNode* child);
    void attachParent(SceneNode* parent);
    void setVertexArray(VertexArray* vArray);
    void setShaderProgram(ShaderProgram* program);
    
    void translate(const glm::vec3& position);
    void rotateX(const float angle);
    void rotateY(const float angle);
    void rotateZ(const float angle);
    void scale(const glm::vec3& m_scale);
    void applyTransformation();

    virtual void render() = 0;

protected:
    void applyTransformation(const glm::mat4& transformation);

    void renderChildren();
    void renderSelf();

    //NodeList  m_children;
    std::list<SceneNode*> m_children;
    SceneNode* m_parent;

    glm::mat4 m_modelMatrix;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    VertexArray* m_vertexArray;
    ShaderProgram* m_shaderProgram;
};