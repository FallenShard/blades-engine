#include "Models/Prism.h"

Prism::Prism()
{
}

Prism::Prism(VertexArray* vArray, ShaderProgram* program)
    : SceneNode(vArray, program)
{
}

Prism::~Prism()
{
}

void Prism::render()
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

    m_vertexArray->bind();
    m_vertexArray->renderIndexed();
}