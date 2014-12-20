#include "Materials/Material.h"
#include "OglWrapper/UniformBuffer.h"

Material::Material()
    : m_uniformBuffer(nullptr)
    , m_program(nullptr)
{

}

void Material::setShaderProgram(ShaderProgram* program)
{
    m_program = program;

    initialize();
}

Material::~Material()
{
    delete m_uniformBuffer;
}

