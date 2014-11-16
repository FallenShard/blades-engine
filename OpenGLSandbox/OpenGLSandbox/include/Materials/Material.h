#pragma once

#include "glm.hpp"

class UniformBuffer;
class ShaderProgram;

class Material
{
public:
    Material();
    virtual ~Material();

    virtual void apply() = 0;


protected:
    UniformBuffer* m_uniformBuffer;
    ShaderProgram* m_program;

};