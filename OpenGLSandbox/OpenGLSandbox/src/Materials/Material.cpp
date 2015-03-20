#include "Materials/Material.h"
#include "OglWrapper/UniformBuffer.h"

namespace fsi
{

Material::Material()
    : m_uniformBuffer(nullptr)
    , m_program(nullptr)
{

}

void Material::setShaderProgram(ShaderProgram* program)
{
    m_program = program;
}

ShaderProgram* Material::getShaderProgram() const
{
    return m_program;
}

Material::~Material()
{
    delete m_uniformBuffer;
}

}