#pragma once

#include "Renderer/ShaderManager.h"
#include "Materials/Material.h"

class PhongMaterial : public Material
{
public:
    PhongMaterial(ShaderProgram* program);
    ~PhongMaterial();

    void setAmbientColor(const glm::vec4& ambient);
    void setDiffuseColor(const glm::vec4& diffuse);
    void setSpecularColor(const glm::vec4& specular);
    void setShininess(float shininess);

    virtual void apply();

private:
    void setupUniformBuffer();

    struct PhongData
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float     shininess;
    };
    
    PhongData m_data;

    GLuint m_uniformBufferId;
    GLint m_uniformBufferIndex;
    std::vector<std::string> m_uniformNames;
    std::vector<GLuint>      m_uniformIndices;
    std::vector<GLint>       m_uniformOffsets;
    GLsizei m_uniformBufferSize;
};