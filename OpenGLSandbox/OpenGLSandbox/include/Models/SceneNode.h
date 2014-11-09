#pragma once

#include "glm.hpp"
#include <list>
#include <memory>

#include "OglWrapper/VertexArray.h"
#include "OglWrapper/ShaderProgram.h"

class SceneNode
{
public:
    SceneNode();
    SceneNode(VertexArray* vArray, ShaderProgram* program);
    virtual ~SceneNode();

    //void attachChild(NodePtr nodePointer);
    void attachChild(SceneNode* child);
    void attachParent(SceneNode* parent);
    void setVertexArray(VertexArray* vArray);
    void setShaderProgram(ShaderProgram* program);
    void setUpdateStatus(bool status);
    
    void setPosition(const glm::vec3& position);
    void translate(const glm::vec3& position);
    void rotate(const float angle, const glm::vec3& axis);
    void setRotationX(const float angle);
    void rotateX(const float angle);
    void setRotationY(const float angle);
    void rotateY(const float angle);
    void setRotationZ(const float angle);
    void rotateZ(const float angle);
    void setScale(const glm::vec3& scale);
    void scale(const glm::vec3& scale);
    void setTransformationMatrix(glm::mat4& matrix);
    void applyTransformation(const glm::mat4& transformation = glm::mat4(1.f));
    const glm::mat4& getModelMatrix() const;

    virtual void render(const glm::mat4& projection, const glm::mat4& view) = 0;

protected:
    void updateRelativeTrans();
    void updateAbsoluteTrans();
    void renderChildren(const glm::mat4& projection, const glm::mat4& view);

    std::list<SceneNode*> m_children;
    SceneNode* m_parent;

    glm::mat4 m_absoluteTrans;
    glm::mat4 m_relativeTrans;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    bool      m_needUpdate;

    VertexArray* m_vertexArray;
    ShaderProgram* m_shaderProgram;
};