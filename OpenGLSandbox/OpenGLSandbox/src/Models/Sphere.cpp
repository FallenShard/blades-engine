#include "Models/Sphere.h"

Sphere::Sphere()
{
    //init();
}

Sphere::Sphere(SphereMesh* mesh, ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_mesh(mesh)
{
    init();
}

Sphere::~Sphere()
{
}

void Sphere::render()
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("modelToWorldMatrix", m_absoluteTrans);
    m_shaderProgram->setUniformAttribute("color", glm::vec4(1.f, 0.5f, 0.f, 1.f));
    
    m_vertexArray->bind();
    m_vertexArray->renderIndexed();
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
    m_shaderProgram->getUniformAttribute("useLight");
    VertexArray::release();
}