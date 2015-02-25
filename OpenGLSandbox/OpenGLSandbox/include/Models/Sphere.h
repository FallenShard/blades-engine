#pragma once

#include "Models/SceneNode.h"

#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/IndexBuffer.h"

#include "Materials/Material.h"

#include "Meshes/SphereMesh.h"

class Sphere : public SceneNode
{
public:
    Sphere();
    Sphere(SphereMesh* mesh, ShaderProgram* program);
    Sphere(SphereMesh* mesh, ShaderProgram* program, Material* material);

    ~Sphere();

    virtual void render(const glm::mat4& projection, const glm::mat4& view);
    void setPrimitiveType(GLenum type);

private:
    void init();

    SphereMesh* m_mesh;
    Material* m_material;
};