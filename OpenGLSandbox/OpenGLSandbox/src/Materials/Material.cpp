#include "Materials/Material.h"
#include "OglWrapper/UniformBuffer.h"

Material::Material()
    : m_uniformBuffer(nullptr)
    , m_program(nullptr)
{

}

Material::~Material()
{
    delete m_uniformBuffer;
}

