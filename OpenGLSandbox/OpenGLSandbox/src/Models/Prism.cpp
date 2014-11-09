#include "Models/Prism.h"

Prism::Prism()
{
}

Prism::Prism(VertexArray* vArray, ShaderProgram* program)
    : SceneNode(vArray, program)
{
}

Prism::Prism(PrismMesh* mesh, ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_mesh(mesh)
{
    init();
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

void Prism::init()
{
    m_vertexArray = m_mesh->getVertexArray();
    m_vertexArray->bind();
    m_vertexArray->enableAttributes(m_shaderProgram->getProgramId());
    VertexArray::release();
}

