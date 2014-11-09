#pragma once

#include "Models/SceneNode.h"
#include "Meshes/CubeMesh.h"

class Prism : public SceneNode
{
public:
    Prism();
    Prism(VertexArray* vertexArray, ShaderProgram* program);
    Prism(PrismMesh* mesh, ShaderProgram* program);
    
    ~Prism();

    virtual void render();

private:
    void init();

    PrismMesh* m_mesh;
};