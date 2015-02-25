#include "Models/Sphere.h"

Sphere::Sphere()
{
    //init();
}

Sphere::Sphere(SphereMesh* mesh, ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_mesh(mesh)
    , m_material(nullptr)
{
    init();
}

Sphere::Sphere(SphereMesh* mesh, ShaderProgram* program, Material* material)
    : SceneNode(nullptr, program)
    , m_mesh(mesh)
    , m_material(material)
{
    init();
}

Sphere::~Sphere()
{
}

void Sphere::render(const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4& MVP = projection * view * m_absoluteTrans;
    glm::mat4& MV = view * m_absoluteTrans;
    glm::mat3& normalMatrix = glm::mat3(glm::inverseTranspose(MV));
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("MVP", MVP);
    m_shaderProgram->setUniformAttribute("MV", MV);
    m_shaderProgram->setUniformAttribute("normalMatrix", normalMatrix);
    
    if (m_material) m_material->apply();
    
    m_vertexArray->bind();
    m_vertexArray->renderIndexed();

    renderChildren(projection, view);
}

void Sphere::setPrimitiveType(GLenum primType)
{
    m_vertexArray->setPrimitiveType(primType);
}

void Sphere::init()
{
    m_vertexArray = m_mesh->getVertexArray();
    m_vertexArray->bind();
    m_vertexArray->enableAttributes(m_shaderProgram->getProgramId());
    VertexArray::release();
}