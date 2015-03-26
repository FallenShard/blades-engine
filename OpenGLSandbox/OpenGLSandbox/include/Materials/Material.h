#pragma once

#include "glm/glm.hpp"
#include "OpenGL.h"

namespace fsi
{

class UniformBuffer;
class ShaderProgram;

class Material
{
public:
    Material();
    virtual ~Material();

    virtual void init() = 0;

    void setShaderProgram(ShaderProgram* program);
    ShaderProgram* getShaderProgram() const;

    virtual void apply() = 0;


protected:
    GLuint m_ubo;

    //UniformBuffer* m_uniformBuffer;
    ShaderProgram* m_program;

};

}