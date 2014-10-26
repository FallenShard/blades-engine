#pragma once

#include "Models/SceneNode.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

class Sphere : public SceneNode
{
public:
    Sphere();
    Sphere(float radius, int rings, int slices, ShaderProgram* program);

    ~Sphere();

    virtual void render();
    void setPrimitiveType(GLenum type);

private:
    void init();

    glm::vec3 m_center;
    float m_radius;
    int m_rings;
    int m_slices;

    VertexBuffer m_buffer;
    IndexBuffer m_indexBuffer;
};