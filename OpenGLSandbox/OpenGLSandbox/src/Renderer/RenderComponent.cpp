#include "Renderer/RenderComponent.h"

namespace fsi
{

namespace
{
    static std::map<VertexAttrib::ID, VertexBufferBinding::ID> initBindingTable()
    {
        std::map<VertexAttrib::ID, VertexBufferBinding::ID> table;

        table[VertexAttrib::Position]  = VertexBufferBinding::Position;
        table[VertexAttrib::Normal]    = VertexBufferBinding::Normal;
        table[VertexAttrib::TexCoord0] = VertexBufferBinding::TexCoord0;

        return table;
    }
}

std::map<VertexAttrib::ID, VertexBufferBinding::ID> RenderComponent::bindingTable = initBindingTable();

RenderComponent::RenderComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
    : m_mesh(mesh)
    , m_material(material)
{
}

void RenderComponent::init()
{
    m_vertexArray.bind();

    m_mesh->bind();

    const std::vector<VertexAttribute>& attribs = m_mesh->getVertexAttributes();

    for (auto attribute : attribs)
    {
        m_vertexArray.enableVertexAttrib(attribute.attribLoc);
        m_vertexArray.attributeFormat(attribute);
        m_vertexArray.attributeBinding(attribute.attribLoc, bindingTable[attribute.attribLoc]);
    }

    m_vertexArray.setVertexCount(m_mesh->getVertexCount());

    m_material->init();
}

void RenderComponent::setPrimitiveType(GLenum primitiveType)
{
    m_vertexArray.setPrimitiveType(primitiveType);
}

void RenderComponent::render()
{
    m_material->apply();
    m_vertexArray.bind();
    m_vertexArray.render();
}

}