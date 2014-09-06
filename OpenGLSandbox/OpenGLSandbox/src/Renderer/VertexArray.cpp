#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

VertexArray::VertexArray()
    : m_vertexCount(0)
    , m_indexType(GL_UNSIGNED_SHORT)
{
    create();
}

VertexArray::VertexArray(GLenum primitiveType)
    : m_primitiveType(primitiveType)
    , m_vertexCount(0)
    , m_indexType(GL_UNSIGNED_SHORT)
{
    create();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::create()
{
    glGenVertexArrays(1, &m_id);
}

void VertexArray::attachBuffer(std::string name, VertexBuffer* buffer)
{
    m_buffers[name] = buffer;
}

void VertexArray::attachIndexBuffer(IndexBuffer* buffer)
{
    m_indexBuffer = buffer;

    m_indexCount = m_indexBuffer->getSize();

    m_indexBuffer->bind();
}

void VertexArray::bind()
{
    glBindVertexArray(m_id);
}

void VertexArray::release()
{
    glBindVertexArray(0);
}

void VertexArray::setPrimitiveType(GLenum primitiveType)
{
    m_primitiveType = primitiveType;
}

GLenum VertexArray::getPrimitiveType() const
{
    return m_primitiveType;
}

void VertexArray::setVertexCount(GLsizei vertexCount)
{
    m_vertexCount = vertexCount;
}

GLsizei VertexArray::getVertexCount() const
{
    return m_vertexCount;
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


