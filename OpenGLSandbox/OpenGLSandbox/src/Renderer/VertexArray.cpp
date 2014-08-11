#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

VertexArray::VertexArray()
    : m_size(0)
{
    create();
}

VertexArray::VertexArray(GLenum primitiveType)
    : m_primitiveType(primitiveType)
    , m_size(0)
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

void VertexArray::attachBuffer(VertexBuffer& buffer)
{
    m_buffers.push_back(&buffer);

    if (m_size == 0)
        m_size = buffer.getSize();
}

void VertexArray::attachBuffers(std::vector<VertexBuffer*> buffers)
{
    m_buffers.insert(m_buffers.end(), buffers.begin(), buffers.end());

    if (m_size == 0)
        m_size = buffers[0]->getSize();
}

void VertexArray::bind()
{
    glBindVertexArray(m_id);
}
