#pragma once

#include "Models/SceneNode.h"

class Prism : public SceneNode
{
public:
    Prism();
    Prism(VertexArray* vertexArray, ShaderProgram* program);
    
    ~Prism();

    virtual void render();
};