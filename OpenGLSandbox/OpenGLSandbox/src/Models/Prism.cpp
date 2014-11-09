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

void Prism::render(const glm::mat4& projection, const glm::mat4& view)
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("MVP", projection * view * m_absoluteTrans);
    m_shaderProgram->setUniformAttribute("MV", view * m_absoluteTrans);

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

