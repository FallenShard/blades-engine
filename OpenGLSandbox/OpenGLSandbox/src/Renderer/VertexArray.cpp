#include "Renderer/VertexArray.h"

namespace
{
    GLuint boundVAO = 0;
}

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

GLsizei VertexArray::getPerVertexDataCount() const
{
    GLsizei count = 0;
    for (auto& attribute : m_attributes)
        count += attribute.second.size;

    return count;
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


