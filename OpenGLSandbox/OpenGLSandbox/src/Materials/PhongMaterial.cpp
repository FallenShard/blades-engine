#include "Materials/PhongMaterial.h"

namespace fsi
{

PhongMaterial::PhongMaterial(glm::vec4& ambient, glm::vec4& diffuse, glm::vec4& specular, float shininess)
{
    m_data.ambient = ambient;
    m_data.diffuse = diffuse;
    m_data.specular = specular;
    m_data.shininess = shininess;
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::setAmbientColor(const glm::vec4& ambient)
{
    m_data.ambient = ambient;
}

void PhongMaterial::setDiffuseColor(const glm::vec4& diffuse)
{
    m_data.diffuse = diffuse;
}

void PhongMaterial::setSpecularColor(const glm::vec4& specular)
{
    m_data.specular = specular;
}

void PhongMaterial::setShininess(float shininess)
{
    m_data.shininess = shininess;

    //m_uniformBuffer->bind();
    //m_uniformBuffer->setBufferData(&m_data, sizeof(PhongData));
}

void PhongMaterial::apply()
{
    //m_uniformBuffer->bindToBindingPoint();
}

void PhongMaterial::init()
{
    glCreateBuffers(1, &m_ubo);

    glNamedBufferData(m_ubo, sizeof PhongData, &m_data, GL_STATIC_DRAW);

    //m_uniformBuffer = new UniformBuffer();
    //m_uniformBuffer->bind();
    //m_uniformBuffer->setBufferData(&m_data, sizeof(PhongData));
    
    std::string blockName = "Material";

    m_program->setUniformBlockBinding(blockName, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
    //m_uniformBuffer->setBindingPoint(0);
    //m_program->setUniformBlockBinding(blockName, 0);
}

}