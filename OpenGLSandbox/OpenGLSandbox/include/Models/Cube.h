#pragma once

#include "Models/SceneNode.h"
#include "Meshes/CubeMesh.h"

namespace fsi
{

class Cube : public SceneNode
{
public:
    Cube();
    Cube(VertexArray* vertexArray, ShaderProgram* program);
    Cube(CubeMesh* mesh, ShaderProgram* program);
    
    ~Cube();

    virtual void render(const glm::mat4& projection, const glm::mat4& view);

private:
    void init();

    CubeMesh* m_mesh;
};

}