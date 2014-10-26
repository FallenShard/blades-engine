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
    m_shaderProgram->setUniformAttribute("modelToWorldMatrix", m_absoluteTrans);

    m_vertexArray->bind();
    m_vertexArray->renderIndexed();
}