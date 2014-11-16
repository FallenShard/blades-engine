#include "Materials/PhongMaterial.h"

PhongMaterial::PhongMaterial(ShaderProgram* program)
{
    m_program = program;
    setupUniformBuffer();
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
}

void PhongMaterial::apply()
{
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PhongData), &m_data, GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uniformBufferId);
    //glUniformBlockBinding(m_program->getProgramId(), m_uniformBufferIndex, 1);
}

void PhongMaterial::setupUniformBuffer()
{
    glGenBuffers(1, &m_uniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferId);

    std::string blockName = "Material";
    m_uniformBufferIndex = glGetUniformBlockIndex(m_program->getProgramId(), blockName.c_str());
    glUniformBlockBinding(m_program->getProgramId(), m_uniformBufferIndex, 1);


    m_uniformNames.push_back(blockName + ".ambient");
    m_uniformNames.push_back(blockName + ".diffuse");
    m_uniformNames.push_back(blockName + ".specular");
    m_uniformNames.push_back(blockName + ".shininess");

    //const GLchar** names = new GLchar*[m_uniformNames.size()];
    //for (int i = 0; i < m_uniformNames.size(); i++)
    //    names[i] = m_uniformNames[i].c_str();

    //glGetUniformIndices(m_program->getProgramId(), 4, names, &m_uniformIndices[0]);
    //glGetActiveUniformsiv(m_program->getProgramId(), 4, m_uniformIndices.data(), GL_UNIFORM_OFFSET, &m_uniformOffsets[0]);
    //glGetActiveUniformBlockiv(m_program->getProgramId(), m_uniformBufferIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_uniformBufferSize);

    m_data.ambient = glm::vec4(0.1f, 0.0f, 0.0f, 1.f);
    m_data.diffuse = glm::vec4(0.1f, 0.1f, 0.0f, 1.f);
    m_data.specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
    m_data.shininess = 0.2f;


    //glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, buffer.data(), GL_DYNAMIC_DRAW);
    //glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
    //glUniformBlockBinding(prog->getProgramId(), uboLoc, 1);
}