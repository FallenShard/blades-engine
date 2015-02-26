#pragma once

#include "glm/glm.hpp"

namespace fsi
{

class UniformBuffer;
class ShaderProgram;

class Material
{
public:
    Material();
    virtual ~Material();

    void setShaderProgram(ShaderProgram* program);

    virtual void apply() = 0;


protected:
    virtual void initialize() = 0;

    UniformBuffer* m_uniformBuffer;
    ShaderProgram* m_program;

};

}