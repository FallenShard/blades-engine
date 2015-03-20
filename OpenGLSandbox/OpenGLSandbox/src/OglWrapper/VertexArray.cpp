#include "OglWrapper/VertexArray.h"

namespace
{
    GLuint boundVAO = 0;
}

namespace fsi
{

VertexArray::VertexArray()
    : m_primitiveType(GL_TRIANGLES)
    , m_vertexCount(0)
    , m_indexType(GL_UNSIGNED_SHORT)
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::VertexArray(GLenum primitiveType)
    : m_primitiveType(primitiveType)
    , m_vertexCount(0)
    , m_indexType(GL_UNSIGNED_SHORT)
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind()
{
    if (m_id != boundVAO)
    {
        glBindVertexArray(m_id);
        boundVAO = m_id;
    }
}

void VertexArray::release()
{
    glBindVertexArray(0);
    boundVAO = 0;
}

GLuint VertexArray::getVertexArrayId() const
{
    return m_id;
}

void VertexArray::setVertexCount(GLsizei vertexCount)
{
    m_vertexCount = vertexCount;
}

GLsizei VertexArray::getVertexCount() const
{
    return m_vertexCount;
}

void VertexArray::setIndexCount(GLsizei indexCount)
{
    m_indexCount = indexCount;
}

GLsizei VertexArray::getIndexCount() const
{
    return m_indexCount;
}

void VertexArray::attachBuffer(std::string name, VertexBuffer* buffer)
{
    m_buffers[name] = buffer;
}

void VertexArray::attachIndexBuffer(IndexBuffer* buffer)
{
    m_indexBuffer = buffer;
    m_indexCount = m_indexBuffer->getSize();
}

void VertexArray::attachAttribute(VertexAttribute attribute)
{
    m_attributes[attribute.name] = attribute;
}

void VertexArray::enableAttributes(GLuint programId)
{
    for (auto& attribute : m_attributes)
        attribute.second.enable(programId);
}

void VertexArray::enableVertexAttrib(GLuint attribLocation)
{
    glEnableVertexAttribArray(attribLocation);
}

void VertexArray::attributeFormat(VertexAttribute attribute)
{
    glVertexAttribFormat(attribute.location, attribute.size, attribute.type, attribute.normalized, attribute.iOffset);
}

void VertexArray::attributeBinding(GLuint attribLocation, GLuint bufferBinding)
{
    glVertexAttribBinding(attribLocation, bufferBinding);
}

void VertexArray::setPrimitiveType(GLenum primitiveType)
{
    m_primitiveType = primitiveType;
}

void VertexArray::render()
{
    glDrawArrays(m_primitiveType, 0, m_vertexCount);
}

void VertexArray::render(GLint first, GLsizei count)
{
    glDrawArrays(m_primitiveType, first, count);
}

void VertexArray::renderIndexed()
{
    glDrawElements(m_primitiveType, m_indexCount, m_indexType, 0);
}

void VertexArray::renderIndexed(GLsizei offset)
{
    glDrawElementsBaseVertex(m_primitiveType, m_indexCount, m_indexType, 0, offset);
}

}