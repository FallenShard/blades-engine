#include "Models/Cube.h"

namespace fsi
{

Cube::Cube()
{
}

Cube::Cube(VertexArray* vArray, ShaderProgram* program)
    : SceneNode(vArray, program)
{
}

Cube::Cube(CubeMesh* mesh, ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_mesh(mesh)
{
    init();
}

Cube::~Cube()
{
}

void Cube::render(const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4& MVP = projection * view * m_absoluteTrans;
    glm::mat4& MV = view * m_absoluteTrans;
    glm::mat3& normalMatrix = glm::mat3(glm::inverseTranspose(MV));
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("MVP", MVP);
    m_shaderProgram->setUniformAttribute("MV", MV);
    m_shaderProgram->setUniformAttribute("normalMatrix", normalMatrix);

    m_vertexArray->bind();
    m_vertexArray->renderIndexed();
}

void Cube::init()
{
    m_vertexArray = m_mesh->getVertexArray();
    m_vertexArray->bind();
    m_vertexArray->enableAttributes(m_shaderProgram->getProgramId());
    VertexArray::release();
}

}